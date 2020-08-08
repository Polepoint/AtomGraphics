//
// Created by neo on 2018/12/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CANVASIMAGESOURCE_H
#define ATOMGRAPHICS_CANVASIMAGESOURCE_H

#include "memory/ScopedRefPtr.h"
#include "CanvasNode.h"
#include "ImageBitmap.h"
#include "graphics/Image.h"

#if PLATFORM(ANDROID)

#include "graphics/GraphicsTypes.h"

#endif

namespace AtomGraphics {

enum SourceImageStatus {
    kNormalSourceImageStatus,
    kUndecodableSourceImageStatus,     // Image element with a 'broken' image
    kZeroSizeCanvasSourceImageStatus,  // Source is a canvas with width or heigh
    // of zero
            kIncompleteSourceImageStatus,  // Image element with no source media
    kInvalidSourceImageStatus,
};

class CanvasNode;

class ImageBitmap;

class CanvasImageSource {

public:

    CanvasImageSource(CanvasNode *node);

    CanvasImageSource(ImageBitmap *image);

    const IntSize &size() const {
        return m_dataSize;
    };

    bool isCanvasNode() const {
        return m_isCanvasNode;
    }

    bool isImageBitmap() const {
        return m_isImageBitmap;
    }

    CanvasNode *getNode() const;

    ImageBitmap *getImage() const;

    bool IsOpaque() const;

#if PLATFORM(IOS)

    scoped_refptr<Image> GetSourceImageForCanvas();

#endif

#if PLATFORM(ANDROID)

    bool IsAccelerated() const;

    scoped_refptr<Image> GetSourceImageForCanvas(AccelerationHint hint);

#endif


private:

    CanvasNode *m_node{nullptr};
    ImageBitmap *m_image{nullptr};

    bool m_isCanvasNode{false};
    bool m_isImageBitmap{false};

    IntSize m_dataSize{IntSize::ZERO};
};

}


#endif //ATOMGRAPHICS_CANVASIMAGESOURCE_H
