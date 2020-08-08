//
// Created by neo on 2020/4/24.
//

#include "CachedImage.h"
#include "cache/MemoryCache.h"
#include "io/WebResourceLoader.h"
#include "node/canvas/ImageBitmap.h"
#include "RequestCache.h"

namespace AtomGraphics {

void CachedImage::addClient(CachedImageClient &client) {
    m_clients.emplace(&client);
}

void CachedImage::removeClient(CachedImageClient &client) {
    m_clients.erase(&client);

    if (deleteIfPossible()) {
        // `this` object is dead here.
        return;
    }

    if (hasClients())
        return;

    auto &requestCache = RequestCache::singleton();
    if (inCache()) {
        requestCache.removeFromLiveResourcesSize(*this);
    }

    requestCache.pruneSoon();
}


void CachedImage::registerHandle(CachedImageHandle *h) {
    ++m_handleCount;
}

void CachedImage::unregisterHandle(CachedImageHandle *h) {
    --m_handleCount;
    if (!m_handleCount)
        deleteIfPossible();
}


bool CachedImage::deleteIfPossible() {
    if (canDelete()) {
        if (!inCache()) {
            delete this;
            return true;
        }
    }

    return false;
}


void CachedImage::load() {
    m_loading = true;
    WebResourceLoader::loadResource(
            m_resourceRequest,
            [this](scoped_refptr<SharedBuffer> sharedBuffer,
                   float imageWidth, float imageHeight,
                   ImageBitmapConfiguration configuration) -> void {
                this->setFailed(!sharedBuffer);
                this->setImageSize(FloatSize(imageWidth, imageHeight));
                this->setData(std::move(sharedBuffer));
                this->setConfiguration(configuration);
                this->finishLoading();
            });
}

void CachedImage::finishLoading() {
    m_loading = false;
    std::set<CachedImageClient *> clients = m_clients;
    for (auto &client: clients) {
        client->notifyFinished(*this);
    }
}

}