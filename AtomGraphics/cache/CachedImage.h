//
// Created by neo on 2020/4/24.
//

#ifndef ATOMGRAPHICS_CACHEDRESOURCE_H
#define ATOMGRAPHICS_CACHEDRESOURCE_H

#include "CachedImageClient.h"
#include "io/SharedBuffer.h"
#include "io/ResourceRequest.h"
#include "math/FloatSize.h"
#include "node/canvas/ImageBitmapConfiguration.h"
#include <set>

namespace AtomGraphics {

class CachedImageHandle;

class CachedImage {

public:

    CachedImage(const ResourceRequest &resourceRequest)
            : m_resourceRequest(resourceRequest) {}

    void addClient(CachedImageClient &client);

    void removeClient(CachedImageClient &);

    void registerHandle(CachedImageHandle *);

    void unregisterHandle(CachedImageHandle *);

    const std::string &url() const { return m_resourceRequest.url(); }

    void load();

    bool loading() const {
        return m_loading;
    }

    bool isFailed() const {
        return m_failed;
    }

    void setFailed(bool failed) {
        m_failed = failed;
    }

    const scoped_refptr<SharedBuffer> &data() const {
        return m_data;
    }

    void setData(const scoped_refptr<SharedBuffer> &data) {
        m_data = data;
    }

    unsigned int size() const {
        if (m_data) {
            return m_data->bufferSize();
        }
        return 0;
    }

    const FloatSize &imageSize() const {
        return m_imageSize;
    }

    void setImageSize(const FloatSize &imageSize) {
        m_imageSize = imageSize;
    }

    const ImageBitmapConfiguration &configuration() const {
        return m_configuration;
    }

    void setConfiguration(const ImageBitmapConfiguration &configuration) {
        m_configuration = configuration;
    }

    // Called by the cache if the object has been removed from the cache
    // while still being referenced. This means the object should delete itself
    // if the number of clients observing it ever drops to 0.
    // The resource can be brought back to cache after successful revalidation.
    void setInCache(bool inCache) { m_inCache = inCache; }

    void finishLoading();

private:
    std::set<CachedImageClient *> m_clients;
    scoped_refptr<SharedBuffer> m_data;
    ImageBitmapConfiguration m_configuration;
    FloatSize m_imageSize{0, 0};
    ResourceRequest m_resourceRequest;

    unsigned m_handleCount{0};
    bool m_inCache: 1;
    bool m_loading = false;
    bool m_failed = false;

    bool hasClients() const { return !m_clients.empty(); }

    bool canDelete() const { return !hasClients() && !m_handleCount; }

    bool inCache() const { return m_inCache; }

    bool deleteIfPossible();
};

}


#endif //ATOMGRAPHICS_CACHEDRESOURCE_H
