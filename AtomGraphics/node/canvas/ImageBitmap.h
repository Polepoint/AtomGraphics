//
// Created by woo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_IMAGEBITMAP_H
#define ATOMGRAPHICS_IMAGEBITMAP_H

#include "platform/AtomPlatformConfig.h"
#include "base/AtomTypes.h"
#include "graphics/ImageData.h"
#include "io/SharedBuffer.h"
#include "io/ImageLoader.h"
#include "Element.h"
#include "ImageBitmapConfiguration.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkImage.h>

#endif

namespace AtomGraphics {

#if PLATFORM(IOS)

class ImageBuffer;

#endif

class ImageBitmap : public Element {

public:

    ImageBitmap();

    ~ImageBitmap();

    float width() const;

    float height() const;

    void setWidth(float width);

    void setHeight(float height);

    const std::string &getSrc() const {
        return m_src;
    }

    void setSrc(const std::string &src) {
        m_src = src;
    }

    void setBuffer(const scoped_refptr<SharedBuffer> &buffer);

    void setConfiguration(const ImageBitmapConfiguration &configuration);

    ImageLoader *imageLoader() const {
        return m_imageLoader.get();
    }

#if PLATFORM(IOS)

    ImageBuffer *imageBuffer();

#elif PLATFORM(ANDROID)

    const SharedBuffer *buffer() {
        return m_buffer.get();
    }

    sk_sp<SkImage> skImage();

#endif

private:

    std::string m_src;
    std::unique_ptr<ImageLoader> m_imageLoader;
    ImageBitmapConfiguration m_configuration;

#if PLATFORM(IOS)

    ImageBuffer *m_imageBuffer;

#elif PLATFORM(ANDROID)

    IntSize m_size;
    void *m_data{nullptr};
    scoped_refptr<SharedBuffer> m_buffer;
    sk_sp<SkImage> m_skImage;

#endif
};

}

#endif //ATOMGRAPHICS_IMAGEBITMAP_H