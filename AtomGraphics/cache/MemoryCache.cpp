//
// Created by neo on 2020/4/24.
//

#include <mutex>
#include "MemoryCache.h"
#include "CachedImage.h"
#include "thread/GraphicsThread.h"

namespace AtomGraphics {

MemoryCache::MemoryCache()
        : m_pruneTimer(GraphicsThread::GraphicsThreadTaskRunner(),
                       *this,
                       &MemoryCache::pruneTimerFired) {

}

CachedImageHandle MemoryCache::resourceForRequest(const CachedImageRequest &request) {
    ResourceCacheKey key(request.url());
    return get(key).m_cachedImageHandle;
}

const unsigned kMaxInactiveResourceCount = 30;

void MemoryCache::add(CachedImageHandle &handle) {
    // m_cache[0] is the LRU entry, so remove it.
    if (m_LURCache.size() == kMaxInactiveResourceCount) {
        ResourceCacheEntry entry = std::move(m_LURCache[0]);
        removeFromLiveResourcesSize(*entry.second.m_cachedImageHandle.get());
        m_deadResource.push_back(entry);
        removeResource(m_LURCache.begin());
    }

    handle->setInCache(true);
    m_liveSize += handle->size();
    m_LURCache.push_back(std::make_pair(ResourceCacheKey(handle->url()), ResourceCacheValue(handle)));
}

void MemoryCache::removeFromLiveResourcesSize(CachedImage &resource) {
    m_liveSize -= resource.size();
    m_deadSize += resource.size();
}

bool MemoryCache::needsPruning() const {
    return m_liveSize + m_deadSize > m_capacity || m_deadSize > m_maxDeadCapacity;
}

void MemoryCache::pruneSoon() {
    if (m_pruneTimer.isActive())
        return;
    if (!needsPruning())
        return;
    m_pruneTimer.startOneShot(0);
}

void MemoryCache::pruneTimerFired() {
    m_deadResource.clear();
    m_deadSize = 0;
    while (needsPruning()) {
        unsigned dataSize = m_LURCache.begin()->second.m_cachedImageHandle->size();
        m_liveSize -= dataSize;
        removeResource(m_LURCache.begin());
    }
}

const ResourceCacheValue &MemoryCache::get(const ResourceCacheKey &key) {

    for (size_t i = 0; i < m_LURCache.size(); ++i) {
        if (m_LURCache[i].first != key)
            continue;

        if (i == m_LURCache.size() - 1)
            return m_LURCache[i].second;

        // If the entry is not the last one, move it to the end of the cache.
        ResourceCacheEntry entry = std::move(m_LURCache[i]);
        removeResource(m_LURCache.begin() + i);
        m_LURCache.push_back(entry);
        return m_LURCache[m_LURCache.size() - 1].second;
    }

    static ResourceCacheValue nullCacheResult;
    return nullCacheResult;
}

void MemoryCache::removeResource(std::vector<ResourceCacheEntry>::iterator p) {
    p->second.m_cachedImageHandle.get()->setInCache(false);
    m_LURCache.erase(p);
}

}