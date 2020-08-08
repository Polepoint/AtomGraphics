//
// Created by neo on 2018/4/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMPATTERN_H
#define ATOMPATTERN_H

#include "memory/ScopedRefPtr.h"
#include "platform/AtomPlatformConfig.h"
#include "AffineTransform.h"
#include "Image.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CoreGraphics.h>

#elif PLATFORM(ANDROID)

#include <skia/core/SkPaint.h>

#endif

namespace AtomGraphics {

class CanvasImageSource;

class Image;

class Pattern {

public:

    Pattern();

    Pattern(CanvasImageSource *source, bool repeatX, bool repeatY);

    Pattern(const Pattern &other);

    Image *image() const;

    bool repeatX() const {
        return m_repeatX;
    }

    bool isRepeatY() const {
        return m_repeatY;
    }

    void setPatternSpaceTransform(const AffineTransform &patternSpaceTransformation) {
        m_patternSpaceTransformation = patternSpaceTransformation;
    }


#if PLATFORM(IOS)

    CGPatternRef createPlatformPattern(const AffineTransform &userSpaceTransformation) const;

#endif

#if PLATFORM(ANDROID)

    void applyToPaint(SkPaint &paint);

#endif

private:

    scoped_refptr<Image> m_tileImage;
    AffineTransform m_patternSpaceTransformation;
    bool m_repeatX{false};
    bool m_repeatY{false};

#if PLATFORM(ANDROID)
    mutable sk_sp<SkShader> m_cachedShader;

    sk_sp<SkShader> createShader();

#endif

};

}


#endif //ATOMPATTERN_H
