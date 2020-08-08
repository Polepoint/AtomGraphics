//
// Created by neo on 2020/4/24.
//

#ifndef ATOMGRAPHICS_MEMORYCACHE_H
#define ATOMGRAPHICS_MEMORYCACHE_H

#include "CachedImage.h"
#include "CachedImageRequest.h"
#include "CachedImageHandle.h"
#include "ResourceCache.h"
#include "TinyLRUCache.h"
#include "thread/Timer.h"

namespace AtomGraphics {

class MemoryCache {

public:

    CachedImageHandle resourceForRequest(const CachedImageRequest &request);

    void add(CachedImageHandle &);

    void removeFromLiveResourcesSize(CachedImage &resource);

    void pruneSoon();

private:
    unsigned m_liveSize{
            0}; // The number of bytes currently consumed by "live" resources in the cache.
    unsigned m_deadSize{
            0}; // The number of bytes currently consumed by "dead" resources in the cache.
    Timer m_pruneTimer;

    MemoryCache();

    bool needsPruning() const;

    void pruneTimerFired();

    std::vector<ResourceCacheEntry> m_LURCache;
    std::vector<ResourceCacheEntry> m_deadResource;
    unsigned m_capacity{0};
    unsigned m_maxDeadCapacity{0};

    const ResourceCacheValue &get(const ResourceCacheKey &key);

    void removeResource(std::vector<ResourceCacheEntry>::iterator);

    friend class RequestCache;

};

}


#endif //ATOMGRAPHICS_MEMORYCACHE_H
