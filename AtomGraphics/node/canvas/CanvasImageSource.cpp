//
// Created by neo on 2018/12/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CanvasImageSource.h"

#if PLATFORM(IOS)

#include "node/canvas/ios/CG/CanvasRenderingContext2DCG.h"

#endif

#if PLATFORM(ANDROID)

#include "node/canvas/android/skia/CanvasRenderingContext2DSkia.h"

#endif

namespace AtomGraphics {

CanvasImageSource::CanvasImageSource(CanvasNode *node) : m_node(node), m_isCanvasNode(true) {
    m_dataSize = node->contentSize();
}

CanvasImageSource::CanvasImageSource(ImageBitmap *image) : m_image(image), m_isImageBitmap(true) {
    m_dataSize.width = static_cast<int>(m_image->width());
    m_dataSize.height = static_cast<int>(m_image->height());
}

CanvasNode *CanvasImageSource::getNode() const {
    return m_node;
}

ImageBitmap *CanvasImageSource::getImage() const {
    return m_image;
}

bool CanvasImageSource::IsOpaque() const {
#if PLATFORM(ANDROID)
    if (m_isImageBitmap) {
        return m_image->skImage()->isOpaque();
    }

#endif
    return false;
}

#if PLATFORM(IOS)

scoped_refptr<Image> CanvasImageSource::GetSourceImageForCanvas() {
    if (m_isImageBitmap) {
        return m_image->imageBuffer()->copyNativeImage(BackingStoreCopy::CopyBackingStore);
    } else {
        CanvasRenderingContext2DCG *c = dynamic_cast<CanvasRenderingContext2DCG *>(m_node->getContext2d().get());
        if (c) {
            return c->buffer()->copyNativeImage(BackingStoreCopy::CopyBackingStore);
        }
    }

    return nullptr;
}


#endif


#if PLATFORM(ANDROID)

bool CanvasImageSource::IsAccelerated() const {
    if (m_isCanvasNode) {
        return dynamic_cast<CanvasRenderingContext2DSkia *>(m_node->getContext2d().get())->IsAccelerated();
    } else {
        return m_image->skImage()->isTextureBacked();
    }
}

scoped_refptr<Image> CanvasImageSource::GetSourceImageForCanvas(AccelerationHint hint) {
    if (m_isImageBitmap) {
        return MakeRefCounted<Image>(m_image->skImage());
    } else {
        CanvasRenderingContext2DSkia *c = dynamic_cast<CanvasRenderingContext2DSkia *>(m_node->getContext2d().get());
        if (c) {
            return MakeRefCounted<Image>(c->GetImage(hint));
        }
    }

    return nullptr;
}

#endif


}
