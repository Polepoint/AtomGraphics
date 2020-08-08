//
// Created by neo on 2019/10/5.
//

#if ENABLE_ACCELERATION

#include "ATIOSurfacePool.h"
#include "thread/GraphicsThread.h"

namespace AtomGraphics {

static const TimeInterval collectionInterval{0.5};
static const TimeInterval kSurfaceAgeBeforeMarkingPurgeable{2};
const size_t kDefaultMaximumBytesCached = 1024 * 1024 * 64;

// We'll never allow more than 1/2 of the cache to be filled with in-use surfaces, because
// they can't be immediately returned when requested (but will be freed up in the future).
const size_t kMaximumInUseBytes = kDefaultMaximumBytesCached / 2;


ATIOSurfacePool &ATIOSurfacePool::sharedPool() {
    static ATIOSurfacePool *pool;
    if (!pool) {
        pool = new ATIOSurfacePool();
    }

    return *pool;
}

static bool surfaceMatchesParameters(ATIOSurface &surface, IntSize requestedSize, CGColorSpaceRef colorSpace, ATIOSurface::Format format) {
    if (format != surface.format())
        return false;
    if (colorSpace != surface.colorSpace())
        return false;
    return !(requestedSize != surface.size());
}

ATIOSurfacePool::ATIOSurfacePool()
        : m_collectionTimer(GraphicsThread::GraphicsThreadTaskRunner(),
                            *this,
                            &ATIOSurfacePool::collectionTimerFired),
          m_bytesCached(0),
          m_inUseBytesCached(0),
          m_maximumBytesCached(kDefaultMaximumBytesCached) {

}

std::unique_ptr<ATIOSurface> ATIOSurfacePool::takeSurface(IntSize size, CGColorSpaceRef colorSpace, ATIOSurface::Format format) {
    auto mapIter = m_cachedSurfaces.find(size);

    if (mapIter == m_cachedSurfaces.end()) {
        return nullptr;
    }

    for (auto surfaceIter = mapIter->second.begin(); surfaceIter != mapIter->second.end(); ++surfaceIter) {
        if (!surfaceMatchesParameters(*surfaceIter->get(), size, colorSpace, format))
            continue;

        auto surface = std::move(*surfaceIter);
        mapIter->second.erase(surfaceIter);

        didUseSurfaceOfSize(size);

        if (mapIter->second.empty()) {
            m_cachedSurfaces.erase(mapIter);
            m_sizesInPruneOrder.pop_back();
        }

        didRemoveSurface(*surface, false);

        surface->setIsVolatile(false);

        return surface;
    }

    // Some of the in-use surfaces may no longer actually be in-use, but we haven't moved them over yet.
    for (auto surfaceIter = m_inUseSurfaces.begin(); surfaceIter != m_inUseSurfaces.end(); ++surfaceIter) {
        if (!surfaceMatchesParameters(*surfaceIter->get(), size, colorSpace, format))
            continue;
        if (surfaceIter->get()->isInUse())
            continue;

        auto surface = std::move(*surfaceIter);
        m_inUseSurfaces.erase(surfaceIter);
        didRemoveSurface(*surface, true);

        surface->setIsVolatile(false);

        return surface;
    }

    return nullptr;
}

void ATIOSurfacePool::addSurface(std::unique_ptr<ATIOSurface> surface) {
    if (!shouldCacheSurface(*surface))
        return;

    bool surfaceIsInUse = surface->isInUse();

    willAddSurface(*surface, surfaceIsInUse);

    if (surfaceIsInUse) {
        m_inUseSurfaces.push_front(std::move(surface));
        scheduleCollectionTimer();
        return;
    }

    insertSurfaceIntoPool(std::move(surface));
}

void ATIOSurfacePool::discardAllSurfaces() {
    m_bytesCached = 0;
    m_inUseBytesCached = 0;
    m_surfaceDetails.clear();
    m_cachedSurfaces.clear();
    m_inUseSurfaces.clear();
    m_sizesInPruneOrder.clear();
    m_collectionTimer.stop();
    platformGarbageCollectNow();
}

void ATIOSurfacePool::setPoolSize(size_t poolSizeInBytes) {
    m_maximumBytesCached = poolSizeInBytes;
    evict(0);
}


bool ATIOSurfacePool::shouldCacheSurface(const ATIOSurface &surface) const {
    if (surface.totalBytes() > m_maximumBytesCached)
        return false;

    // There's no reason to pool empty surfaces; we should never allocate them in the first place.
    // This also covers isZero(), which would cause trouble when used as the key in m_cachedSurfaces.
    return !surface.size().isEmpty();

}

void ATIOSurfacePool::willAddSurface(ATIOSurface &surface, bool inUse) {
    CachedSurfaceDetails &details = m_surfaceDetails.emplace(&surface, CachedSurfaceDetails()).first->second;
    details.resetLastUseTime();

    surface.releaseGraphicsContext();

    size_t surfaceBytes = surface.totalBytes();

    evict(surfaceBytes);

    m_bytesCached += surfaceBytes;
    if (inUse)
        m_inUseBytesCached += surfaceBytes;
}

void ATIOSurfacePool::didRemoveSurface(ATIOSurface &surface, bool inUse) {
    size_t surfaceBytes = surface.totalBytes();
    m_bytesCached -= surfaceBytes;
    if (inUse)
        m_inUseBytesCached -= surfaceBytes;

    m_surfaceDetails.erase(&surface);
}

void ATIOSurfacePool::didUseSurfaceOfSize(IntSize size) {
    m_sizesInPruneOrder.erase(std::find(m_sizesInPruneOrder.rbegin(), m_sizesInPruneOrder.rend(), size).base());
    m_sizesInPruneOrder.push_back(size);
}

void ATIOSurfacePool::insertSurfaceIntoPool(std::unique_ptr<ATIOSurface> surface) {
    IntSize surfaceSize = surface->size();
    auto insertedTuple = m_cachedSurfaces.emplace(surfaceSize, CachedSurfaceQueue());
    insertedTuple.first->second.push_front(std::move(surface));
    if (!insertedTuple.second)
        m_sizesInPruneOrder.erase(std::find(m_sizesInPruneOrder.rbegin(), m_sizesInPruneOrder.rend(), surfaceSize).base());
    m_sizesInPruneOrder.push_back(surfaceSize);

    scheduleCollectionTimer();
}

void ATIOSurfacePool::evict(size_t additionalSize) {
    if (additionalSize >= m_maximumBytesCached) {
        discardAllSurfaces();
        return;
    }

    // FIXME: Perhaps purgeable surfaces should count less against the cap?
    // We don't want to end up with a ton of empty (purged) surfaces, though, as that would defeat the purpose of the pool.
    size_t targetSize = m_maximumBytesCached - additionalSize;

    // Interleave eviction of old cached surfaces and more recent in-use surfaces.
    // In-use surfaces are more recently used, but less useful in the pool, as they aren't
    // immediately available when requested.
    while (m_bytesCached > targetSize) {
        tryEvictOldestCachedSurface();

        if (m_inUseBytesCached > kMaximumInUseBytes || m_bytesCached > targetSize)
            tryEvictInUseSurface();
    }

    while (m_inUseBytesCached > kMaximumInUseBytes || m_bytesCached > targetSize)
        tryEvictInUseSurface();
}

void ATIOSurfacePool::tryEvictInUseSurface() {
    if (m_inUseSurfaces.empty())
        return;

    auto &surface = m_inUseSurfaces.front();
    didRemoveSurface(*surface, true);
}

void ATIOSurfacePool::tryEvictOldestCachedSurface() {
    if (m_cachedSurfaces.empty())
        return;

    if (m_sizesInPruneOrder.empty())
        return;

    CachedSurfaceMap::iterator surfaceQueueIter = m_cachedSurfaces.find(m_sizesInPruneOrder.front());
    auto &surface = surfaceQueueIter->second.front();
    didRemoveSurface(*surface, false);

    if (surfaceQueueIter->second.empty()) {
        m_cachedSurfaces.erase(surfaceQueueIter);
        m_sizesInPruneOrder.erase(m_sizesInPruneOrder.begin());
    }
}

void ATIOSurfacePool::scheduleCollectionTimer() {
    if (!m_collectionTimer.isActive())
        m_collectionTimer.startRepeating(collectionInterval);
}

void ATIOSurfacePool::collectionTimerFired() {
    collectInUseSurfaces();
    bool markedAllSurfaces = markOlderSurfacesPurgeable();

    if (!m_inUseSurfaces.size() && markedAllSurfaces)
        m_collectionTimer.stop();

    platformGarbageCollectNow();
}

void ATIOSurfacePool::collectInUseSurfaces() {
    CachedSurfaceQueue newInUseSurfaces;
    for (CachedSurfaceQueue::iterator surfaceIter = m_inUseSurfaces.begin(); surfaceIter != m_inUseSurfaces.end(); ++surfaceIter) {
        ATIOSurface *surface = surfaceIter->get();
        if (surface->isInUse()) {
            newInUseSurfaces.push_back(std::move(*surfaceIter));
            continue;
        }

        m_inUseBytesCached -= surface->totalBytes();
        insertSurfaceIntoPool(std::move(*surfaceIter));
    }

    m_inUseSurfaces = std::move(newInUseSurfaces);
}

bool ATIOSurfacePool::markOlderSurfacesPurgeable() {
    bool markedAllSurfaces = true;
    auto markTime = Clock::now();

    for (auto &surfaceAndDetails : m_surfaceDetails) {
        if (surfaceAndDetails.second.hasMarkedPurgeable)
            continue;

        if (markTime - surfaceAndDetails.second.lastUseTime < kSurfaceAgeBeforeMarkingPurgeable) {
            markedAllSurfaces = false;
            continue;
        }

        surfaceAndDetails.first->setIsVolatile(true);
        surfaceAndDetails.second.hasMarkedPurgeable = true;
    }

    return markedAllSurfaces;
}

void ATIOSurfacePool::platformGarbageCollectNow() {
    //TODO: performing in the main thread?
    if (GraphicsThread::IsGraphicsThread())
        return;

    // We need to trigger a CA commit in the web process to trigger the release layer-related memory, since the WebProcess doesn't normally do CA commits.
    [CATransaction begin];
    [CATransaction commit];
}

}

#endif