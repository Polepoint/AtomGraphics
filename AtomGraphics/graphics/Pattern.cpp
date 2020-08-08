//
// Created by neo on 2018/4/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <string>
#include "Pattern.h"
#include "node/canvas/CanvasImageSource.h"

#if PLATFORM(IOS)

#include "CanvasNodeCG.h"
#include "ImageBuffer.h"

#endif

#if PLATFORM(ANDROID)

#include "node/canvas/android/skia/CanvasNodeSkia.h"

#endif

namespace AtomGraphics {

Pattern::Pattern() {

}

Pattern::Pattern(CanvasImageSource *source, bool repeatX, bool repeatY) {

#if PLATFORM(IOS)
    m_tileImage = source->GetSourceImageForCanvas();
#elif PLATFORM(ANDROID)
    m_tileImage = source->GetSourceImageForCanvas(kPreferAcceleration);

#endif

    m_repeatX = repeatX;
    m_repeatY = repeatY;
}

Pattern::Pattern(const Pattern &other) {
    m_tileImage = other.m_tileImage;
    m_repeatX = other.m_repeatX;
    m_repeatY = other.m_repeatY;
}

Image *Pattern::image() const {
    return m_tileImage.get();
}

}
