//
// Created by neo on 2019/10/5.
//

#ifndef ATOMGRAPHICS_ATIOSURFACEPOOL_H
#define ATOMGRAPHICS_ATIOSURFACEPOOL_H

#if ENABLE_ACCELERATION

#include <queue>
#include "ATIOSurface.h"
#include "thread/Timer.h"

namespace AtomGraphics {

class ATIOSurfacePool {

public:

    static ATIOSurfacePool &sharedPool();

    std::unique_ptr<ATIOSurface> takeSurface(IntSize, CGColorSpaceRef, ATIOSurface::Format);

    void addSurface(std::unique_ptr<ATIOSurface>);

    void discardAllSurfaces();

    void setPoolSize(size_t);

private:

    struct CachedSurfaceDetails {
        CachedSurfaceDetails()
                : hasMarkedPurgeable(false) {}

        void resetLastUseTime() { lastUseTime = Clock::now(); }

        TimeInterval lastUseTime;
        bool hasMarkedPurgeable;
    };

    Timer m_collectionTimer;
    typedef std::deque<std::unique_ptr<ATIOSurface>> CachedSurfaceQueue;
    CachedSurfaceQueue m_inUseSurfaces;
    typedef std::map<IntSize, CachedSurfaceQueue> CachedSurfaceMap;
    CachedSurfaceMap m_cachedSurfaces;
    typedef std::map<ATIOSurface *, CachedSurfaceDetails> CachedSurfaceDetailsMap;
    CachedSurfaceDetailsMap m_surfaceDetails;
    std::vector<IntSize> m_sizesInPruneOrder;

    size_t m_bytesCached;
    size_t m_inUseBytesCached;
    size_t m_maximumBytesCached;

    ATIOSurfacePool();

    bool shouldCacheSurface(const ATIOSurface &) const;

    void willAddSurface(ATIOSurface &, bool inUse);

    void didRemoveSurface(ATIOSurface &, bool inUse);

    void didUseSurfaceOfSize(IntSize);

    void insertSurfaceIntoPool(std::unique_ptr<ATIOSurface>);

    void evict(size_t additionalSize);

    void tryEvictInUseSurface();

    void tryEvictOldestCachedSurface();

    void scheduleCollectionTimer();

    void collectionTimerFired();

    void collectInUseSurfaces();

    bool markOlderSurfacesPurgeable();

    void platformGarbageCollectNow();
};

}

#endif

#endif //ATOMGRAPHICS_ATIOSURFACEPOOL_H
