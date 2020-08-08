//
// Created by neo on 2020/4/22.
//

#include "CachedImageLoader.h"
#include "MemoryCache.h"
#include "CachedImageRequest.h"
#include "RequestCache.h"

namespace AtomGraphics {

CachedImageHandle CachedImageLoader::requestImage(CachedImageRequest &request) {


    auto &requestCache = RequestCache::singleton();

    // See if we can use an existing resource from the cache.
    CachedImageHandle resource = requestCache.resourceForRequest(request);

    if (!resource) {
        resource = loadResource(request);
    }

    return resource;
}


CachedImageHandle CachedImageLoader::loadResource(CachedImageRequest &request) {

    CachedImageHandle resource(new CachedImage(request));

    auto &requestCache = RequestCache::singleton();

    requestCache.add(resource);

    resource->load();

    return resource;
}


}