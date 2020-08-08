//
// Created by neo on 2020/4/30.
//

#ifndef ATOMGRAPHICS_RESOURCECACHE_H
#define ATOMGRAPHICS_RESOURCECACHE_H

#include <string>
#include "CachedImageHandle.h"

namespace AtomGraphics {

struct ResourceCacheKey {

    ResourceCacheKey(const std::string &url) : m_url(url) {}

    bool operator==(const ResourceCacheKey &other) const {
        return m_url == other.m_url;
    }

    bool operator!=(const ResourceCacheKey &other) const {
        return !(*this == other);
    }

    bool operator<(const ResourceCacheKey &other) const {
        return m_url < other.m_url;
    }

    std::string m_url;
};

struct ResourceCacheValue {

    ResourceCacheValue() = default;

    ResourceCacheValue(CachedImageHandle handle) : m_cachedImageHandle(handle) {};

    ResourceCacheValue(const ResourceCacheValue &other) {
        m_cachedImageHandle = other.m_cachedImageHandle;
    }

    CachedImageHandle m_cachedImageHandle;
};


typedef std::pair<ResourceCacheKey, ResourceCacheValue> ResourceCacheEntry;

}


#endif //ATOMGRAPHICS_RESOURCECACHE_H
