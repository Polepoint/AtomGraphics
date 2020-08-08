//
// Created by neo on 2019/3/30.
//

#include "GLResourceManager.h"

namespace AtomGraphics {

GPUResourceManager *GPUResourceManager::sharedManger() {
    static GPUResourceManager *manager = nullptr;
    if (!manager) {
        manager = new GPUResourceManager;
    }

    return manager;
}

scoped_refptr<GLResource> GPUResourceManager::resourceForPage(long pageID) {
    auto it = m_resources.find(pageID);
    scoped_refptr<GLResource> targetResource = nullptr;
    if (it != m_resources.end()) {
        targetResource = it->second;
    }

    return targetResource;
}

void GPUResourceManager::initializePageResource(long pageID, ANativeWindow *window) {
    auto it = m_resources.find(pageID);
    if (it == m_resources.end()) {
        m_resources.emplace(pageID, MakeRefCounted<GLResource>(window));
    }
}

void GPUResourceManager::removePageResource(long pageID) {
    auto it = m_resources.find(pageID);
    if (it != m_resources.end()) {
        it->second->destroy();
        m_resources.erase(it);
    }
}

}