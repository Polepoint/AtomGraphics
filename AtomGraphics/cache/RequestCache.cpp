//
// Created by neo on 2020/4/26.
//

#include "RequestCache.h"


namespace AtomGraphics {

RequestCache &RequestCache::singleton() {
    static RequestCache *requestCache;

    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        requestCache = new RequestCache;
    });

    return *requestCache;
}


RequestCache::RequestCache()
        : m_diskCache(new DiskCache),
          m_memoryCache(new MemoryCache) {

}

CachedImageHandle RequestCache::resourceForRequest(const CachedImageRequest &request) {

    CachedImageHandle handle = m_memoryCache->resourceForRequest(request);
    if (!handle) {
        handle = m_diskCache->resourceForRequest(request);
    }

    if (!handle) {
        auto it = std::find_if(m_deferredResources.begin(), m_deferredResources.end(),
                               [request](ResourceCacheEntry &entry) -> bool {
                                   return entry.first.m_url == request.url();
                               });
        if (it != m_deferredResources.end()) {
            handle = it->second.m_cachedImageHandle;
        }
    }

    return handle;
}

const unsigned kMaxMemoryCacheResourceSize = 10 * 1024 * 1024; // 10MB

void RequestCache::add(CachedImageHandle &handle) {

    handle->addClient(*this);

    m_deferredResources.push_back(std::make_pair(ResourceCacheKey(handle->url()), ResourceCacheValue(handle)));
}

void RequestCache::removeFromLiveResourcesSize(CachedImage &resource) {
    m_memoryCache->removeFromLiveResourcesSize(resource);
}

void RequestCache::pruneSoon() {
    m_memoryCache->pruneSoon();
}


void RequestCache::notifyFinished(CachedImage &image) {

    CachedImage *targetImage = &image;
    auto it = std::find_if(m_deferredResources.begin(), m_deferredResources.end(),
                           [targetImage](ResourceCacheEntry &entry) -> bool {
                               return entry.second.m_cachedImageHandle.get() == targetImage;
                           });

    if (it != m_deferredResources.end()) {
        CachedImageHandle &handle = it->second.m_cachedImageHandle;
        if (handle->size() < kMaxMemoryCacheResourceSize) {
            m_memoryCache->add(handle);
        }
        m_diskCache->add(handle);
        handle->addClient(*this);
        m_deferredResources.erase(it);
    }
}

}