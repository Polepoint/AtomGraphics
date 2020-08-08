//
// Created by neo on 2020/4/22.
//

#include "ImageLoader.h"
#include "base/StringUtils.h"
#include "cache/CachedImageLoader.h"
#include "node/canvas/EventSender.h"
#include "node/canvas/ImageBitmap.h"

namespace AtomGraphics {


ImageLoader::~ImageLoader() {
    if (m_image) {
        m_image->removeClient(*this);
    }
}

void ImageLoader::updateFromElement() {
    auto &url = m_element.getSrc();

    if (!m_failedLoadURL.empty() && m_failedLoadURL == url) {
        return;
    }

    CachedImageHandle newImage = nullptr;

    if (!url.empty() && !StringUtils::stripLeadingAndTrailingSpaces(url).empty()) {
        CachedImageRequest request(url);

        newImage = CachedImageLoader::requestImage(request);

        if (!newImage) {
            m_failedLoadURL = url;
            EventSender::sender()->dispatchEventSoon(EventType::EventTypeOnError, &this->element());
        } else {
            clearFailedLoadURL();
            if (!newImage->loading()) {
                updateElementImage(*newImage.get());
            }
        }
    } else {
        m_failedLoadURL = url;
    }

    CachedImage *oldImage = m_image.get();

    if (newImage != m_image) {
        m_image = newImage;

        if (newImage) {
            newImage->addClient(*this);
        }

        if (oldImage) {
            oldImage->removeClient(*this);
        }
    }
}

void ImageLoader::notifyFinished(CachedImage &cachedImage) {
    updateElementImage(cachedImage);
}

void ImageLoader::updateElementImage(CachedImage &cachedImage) {
    element().setWidth(cachedImage.imageSize().width);
    element().setHeight(cachedImage.imageSize().height);
    element().setBuffer(cachedImage.data());
    element().setConfiguration(cachedImage.configuration());

    if (cachedImage.isFailed()) {
        EventSender::sender()->dispatchEventSoon(EventType::EventTypeOnError, &this->element());
    } else {
        EventSender::sender()->dispatchEventSoon(EventType::EventTypeOnLoad, &this->element());
    }
}

inline void ImageLoader::clearFailedLoadURL() {
    m_failedLoadURL = "";
}

}