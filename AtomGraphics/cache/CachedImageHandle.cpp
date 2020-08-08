//
// Created by neo on 2020/4/23.
//

#include "CachedImageHandle.h"


namespace AtomGraphics {

CachedImageHandle::~CachedImageHandle() {
    if (m_resource) {
        m_resource->unregisterHandle(this);
    }
}

void CachedImageHandle::setResource(CachedImage *resource) {
    if (resource == m_resource)
        return;
    if (m_resource)
        m_resource->unregisterHandle(this);
    m_resource = resource;
    if (m_resource)
        m_resource->registerHandle(this);
}

}