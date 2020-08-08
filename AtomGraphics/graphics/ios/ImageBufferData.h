//
// Created by neo on 2018/12/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_IMAGEBUFFERDATA_H
#define ATOMGRAPHICS_IMAGEBUFFERDATA_H


#import <AtomGraphics/ScopedRefPtr.h>
#include "math/IntSize.h"
#include "math/IntRect.h"
#include "graphics/GraphicsTypes.h"
#import "SharedBuffer.h"

#if ENABLE_ACCELERATION
#include "graphics/ios/accelerate/ATIOSurface.h"
#endif

namespace AtomGraphics {

class ImageBufferData {

public:

    ImageBufferData(const IntSize &bufferSize, size_t bytesPerRow);

    ImageBufferData(void *data, const IntSize &bufferSize, size_t bytesPerRow);

    ~ImageBufferData() {
#if ENABLE_ACCELERATION
        if (m_surface) {
            AtomGraphics::ATIOSurface::moveToPool(std::move(m_surface));
        }
#endif
    }

    const IntSize &bufferSize() const {
        return m_bufferImageSize;
    }

    void *data() const {
        return m_buffer->data();
    }

    size_t bytesCount() const {
        return m_buffer->bufferSize();
    }

    std::unique_ptr<ImageBufferData> getData(AlphaPremultiplication outputFormat, const IntRect &rect, const IntSize &size) const;

    void putData(const void *, AlphaPremultiplication sourceFormat, const IntSize &, const IntRect &, const IntPoint &);

private:

    ImageBufferData(const IntSize &bufferSize) : m_bufferImageSize(bufferSize) {};

    IntSize m_bufferImageSize{IntSize::ZERO};
    size_t m_bytesPerRow{0};
    scoped_refptr<SharedBuffer> m_buffer;

#if ENABLE_ACCELERATION
    std::unique_ptr<ATIOSurface> m_surface;
#endif

    friend class ImageBuffer;

    friend class ImageBitmap;
};

}

#endif //ATOMGRAPHICS_IMAGEBUFFERDATA_H
