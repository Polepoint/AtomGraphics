//
// Created by neo on 2018/12/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_IMAGE_H
#define ATOMGRAPHICS_IMAGE_H

#include <stddef.h>
#include "math/IntSize.h"
#include "memory/RefCounted.h"

#if PLATFORM(IOS)

#include "memory/ios/RetainPtr.h"

#endif

#if PLATFORM(ANDROID)

#include <skia/core/SkImage.h>

#endif

namespace AtomGraphics {

class Image : public RefCounted<Image> {

public:

#if PLATFORM(IOS)

    Image(RetainPtr<CGImageRef> image);

    CGImageRef getCGImage()const ;

#elif PLATFORM(ANDROID)

    Image(sk_sp<SkImage> image);

    sk_sp<SkImage> getSkImage();

#endif

    ~Image();

    size_t width() const;

    size_t height() const;

    IntSize size() const;

    bool isOpaque() const;

protected:
    size_t m_width{0};
    size_t m_height{0};

#if PLATFORM(IOS)
    RetainPtr<CGImageRef> m_image;
#elif PLATFORM(ANDROID)
    sk_sp<SkImage> m_skImage;
#endif

};

}


#endif //ATOMGRAPHICS_IMAGE_H
