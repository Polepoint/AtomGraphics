//
// Created by neo on 2020/4/22.
//

#ifndef ATOMGRAPHICS_IMAGELOADER_H
#define ATOMGRAPHICS_IMAGELOADER_H

#include <memory>
#include <string>
#include "platform/AtomPlatformConfig.h"
#include "ResourceRequest.h"
#include "cache/CachedImageClient.h"
#include "cache/CachedImageHandle.h"

namespace AtomGraphics {

class ImageBitmap;

class ImageLoader : public CachedImageClient {

public:

    ImageLoader(ImageBitmap &element) : m_element(element) {};

    ~ImageLoader();

    void updateFromElement();

private:

    ImageBitmap &m_element;
    std::string m_failedLoadURL;
    CachedImageHandle m_image;

    ImageBitmap &element() {
        return m_element;
    }

    void notifyFinished(CachedImage &) override;

    void updateElementImage(CachedImage &);

    void clearFailedLoadURL();

    friend ImageBitmap;

};

}


#endif //ATOMGRAPHICS_IMAGELOADER_H
