//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMIMAGEBUFFER_H
#define ATOMIMAGEBUFFER_H

#include "memory/RefCounted.h"
#include "platform/AtomPlatformConfig.h"
#include "graphics/ImageData.h"
#include "graphics/ios/ImageBufferData.h"
#include "graphics/ios/CG/GraphicsContextCG.h"
#include "AffineTransform.h"
#include <CoreGraphics/CoreGraphics.h>
#include <cstddef>
#include <mutex>
#include "node/canvas/ImageBitmapConfiguration.h"

#if ENABLE_ACCELERATION
#include "graphics/ios/accelerate/ATIOSurface.h"
#endif

namespace AtomGraphics {

enum BackingStoreCopy {
    CopyBackingStore, // Guarantee subsequent draws don't affect the copy.
    DontCopyBackingStore // Subsequent draws may affect the copy.
};

class ImageBuffer final : public RefCounted<ImageBuffer> {

public:

    REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE();

    enum CoordinateSystem {
        LogicalCoordinateSystem, BackingStoreCoordinateSystem
    };

    static ImageBuffer *Create(
            IntSize size,
            size_t bytesPerPixel = 4,
            float resolutionScale = 1,
            AccelerationHint accelerationHint = kPreferNoAcceleration);

    static ImageBuffer *Create(
            std::unique_ptr<ImageBufferData> data,
            IntSize size,
            size_t bytesPerPixel = 4,
            float resolutionScale = 1,
            AccelerationHint accelerationHint = kPreferNoAcceleration);

    static scoped_refptr<ImageBuffer> CreateScopedRef(
            IntSize size,
            size_t bytesPerPixel = 4,
            float resolutionScale = 1,
            AccelerationHint accelerationHint = kPreferNoAcceleration);

    static scoped_refptr<ImageBuffer> CreateScopedRef(std::unique_ptr<ImageBufferData> data, IntSize size, size_t bytesPerPixel = 4, float resolutionScale = 1);

    bool accelerated() const {
        return m_accelerationHint == kPreferAcceleration;
    }

    size_t getHeight() const {
        return static_cast<size_t>(m_imageSize.height);
    }

    size_t getWidth() const {
        return static_cast<size_t>(m_imageSize.width);
    }

    IntSize size() const {
        return m_imageSize;
    }

    IntSize resolutionSize() const {
        IntSize size = IntSize(m_imageSize);
        size.scale(m_resolutionScale);
        return size;
    }

#if ENABLE_ACCELERATION
    ATIOSurface *surface() const {
        return m_data->m_surface.get();
    }
#endif

    std::unique_ptr<GraphicsContextCG> createGraphicsContext();

    RetainPtr<CGImageRef> makeCGImage();

    scoped_refptr<ImageData> getUnmultipliedImageData(const IntRect &, IntSize *pixelArrayDimensions = nullptr,
                                                        CoordinateSystem = LogicalCoordinateSystem) const;

    void putByteArray(const ImageData &, const IntRect &sourceRect, const IntPoint &destPoint,
                      ImageBuffer::CoordinateSystem);


    scoped_refptr<Image> copyNativeImage(BackingStoreCopy copyBehavior = BackingStoreCopy::DontCopyBackingStore);

    AffineTransform baseTransform() const;

    FloatSize sizeForDestinationSize(FloatSize destinationSize) const;

    const ImageBufferData *bufferData() const {
        return m_data.get();
    }

    void retain();

    void release();

private:

    std::unique_ptr<ImageBufferData> m_data;

    float m_resolutionScale{1};
    AccelerationHint m_accelerationHint;
    size_t m_bytesPerPixel;
    IntSize m_imageSize;

    ImageBitmapConfiguration m_configuration;

    std::mutex m_lock;

    static ImageBuffer *CreateEmptyImageBuffer();

    ImageBuffer() {};

    ImageBuffer(IntSize size, size_t bytesPerPixel, float resolutionScale = 1, AccelerationHint accelerationHint = AccelerationHint::kPreferNoAcceleration);

    ImageBuffer(std::unique_ptr<ImageBufferData> data, IntSize size, size_t bytesPerPixel, float resolutionScale = 1, AccelerationHint accelerationHint = AccelerationHint::kPreferNoAcceleration);

    void flushContext() const;

    static void releaseBitmapContextData(void *typelessBitmap, void *typelessData);

    static void releaseDataProviderData(void *typelessBitmap, const void *typelessData, size_t);

    friend class ImageBitmap;

    friend class RefCounted;
};

}

#endif //ATOMIMAGEBUFFER_H
