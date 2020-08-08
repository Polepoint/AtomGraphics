//
// Created by neo on 2020/4/26.
//

#ifndef ATOMGRAPHICS_REQUESTCACHE_H
#define ATOMGRAPHICS_REQUESTCACHE_H

#include "CachedImageHandle.h"
#include "CachedImageRequest.h"
#include "MemoryCache.h"
#include "DiskCache.h"

namespace AtomGraphics {

class RequestCache : public CachedImageClient {

public:

    static RequestCache &singleton();

    CachedImageHandle resourceForRequest(const CachedImageRequest &request);

    void add(CachedImageHandle &);

    void removeFromLiveResourcesSize(CachedImage &resource);

    void pruneSoon();

private:

    RequestCache();

    std::unique_ptr<DiskCache> m_diskCache;
    std::unique_ptr<MemoryCache> m_memoryCache;

    std::vector<ResourceCacheEntry> m_deferredResources;

    void notifyFinished(CachedImage &image) override;

};

}


#endif //ATOMGRAPHICS_REQUESTCACHE_H
