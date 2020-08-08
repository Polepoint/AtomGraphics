//
// Created by neo on 2020/4/23.
//

#ifndef ATOMGRAPHICS_CACHEDIMAGEHANDLE_H
#define ATOMGRAPHICS_CACHEDIMAGEHANDLE_H

#include <string>
#include <set>
#include "CachedImageClient.h"
#include "CachedImage.h"

namespace AtomGraphics {

class CachedImageHandle final {

public:

    CachedImageHandle() {}

    CachedImageHandle(CachedImage *resource) {
        setResource(resource);
    }

    ~CachedImageHandle();

    CachedImage *get() const { return m_resource; }

    operator bool() const { return m_resource != nullptr; }

    CachedImageHandle &operator=(const CachedImageHandle &o) {
        setResource(o.get());
        return *this;
    }

    bool operator!=(const CachedImageHandle &other) const {
        return m_resource != other.m_resource;
    }

    bool operator==(const CachedImageHandle &other) const {
        return m_resource == other.m_resource;
    }

    CachedImage *operator->() const { return get(); }

private:

    CachedImage *m_resource{nullptr};

    void setResource(CachedImage *);
};

}


#endif //ATOMGRAPHICS_CACHEDIMAGEHANDLE_H
