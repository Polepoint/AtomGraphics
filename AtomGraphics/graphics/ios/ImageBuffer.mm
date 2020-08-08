//
// Created by neo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "ImageBuffer.h"
#include "ColorCG.h"

namespace AtomGraphics {

ImageBuffer *ImageBuffer::Create(IntSize size, size_t bytesPerPixel, float resolutionScale, AccelerationHint accelerationHint) {
    return new ImageBuffer(size, bytesPerPixel, resolutionScale, accelerationHint);
}

ImageBuffer *ImageBuffer::Create(
        std::unique_ptr<ImageBufferData> data,
        IntSize size,
        size_t bytesPerPixel,
        float resolutionScale,
        AccelerationHint accelerationHint) {

    return new ImageBuffer(std::move(data), size, bytesPerPixel, resolutionScale, accelerationHint);
}

scoped_refptr<ImageBuffer> ImageBuffer::CreateScopedRef(IntSize size, size_t bytesPerPixel, float resolutionScale, AccelerationHint accelerationHint) {
    ImageBuffer *bufferPtr = Create(size, bytesPerPixel, resolutionScale, accelerationHint);
    scoped_refptr<ImageBuffer> buffer(bufferPtr);
    bufferPtr->release();
    return buffer;
}

scoped_refptr<ImageBuffer> ImageBuffer::CreateScopedRef(
        std::unique_ptr<ImageBufferData> data,
        IntSize size, size_t bytesPerPixel, float resolutionScale) {
    ImageBuffer *bufferPtr = Create(std::move(data), size, bytesPerPixel, resolutionScale);
    scoped_refptr<ImageBuffer> buffer(bufferPtr);
    bufferPtr->release();
    return buffer;
}

ImageBuffer *ImageBuffer::CreateEmptyImageBuffer() {
    return new ImageBuffer(IntSize::ZERO, 4, 1);
}

ImageBuffer::ImageBuffer(IntSize size, size_t bytesPerPixel, float resolutionScale, AccelerationHint accelerationHint)
        : m_imageSize(size),
          m_bytesPerPixel(bytesPerPixel),
          m_resolutionScale(resolutionScale),
          m_accelerationHint(accelerationHint) {
    IntSize scaledSize = m_imageSize;
    scaledSize.scale(m_resolutionScale);
    if (m_accelerationHint == kPreferAcceleration) {
        m_data.reset(new ImageBufferData(nullptr, scaledSize, scaledSize.width * m_bytesPerPixel));
        FloatSize userBounds = sizeForDestinationSize(FloatSize(scaledSize.width, m_imageSize.height));
#if ENABLE_ACCELERATION
        m_data->m_surface = ATIOSurface::create(scaledSize, IntSize(userBounds), sRGBColorSpaceRef());
        if (m_data->m_surface) {
            RetainPtr<CGContextRef> cgContext = m_data->m_surface->ensurePlatformContext();
            if (cgContext)
                CGContextClearRect(cgContext.get(), FloatRect(FloatPoint(), userBounds));
            else {
                m_data->m_surface = nullptr;
                m_accelerationHint = kPreferNoAcceleration;
            }

        } else {
            m_accelerationHint = kPreferNoAcceleration;
        }
#else
        m_accelerationHint = kPreferNoAcceleration;
#endif
    }

    if (m_accelerationHint == kPreferNoAcceleration) {
        m_data.reset(new ImageBufferData(scaledSize, scaledSize.width * m_bytesPerPixel));
    }
}

ImageBuffer::ImageBuffer(std::unique_ptr<ImageBufferData> data, IntSize size, size_t bytesPerPixel, float resolutionScale, AccelerationHint accelerationHint)
        : m_data(std::move(data)),
          m_imageSize(size),
          m_bytesPerPixel(bytesPerPixel),
          m_resolutionScale(resolutionScale),
          m_accelerationHint(accelerationHint) {

}

void ImageBuffer::flushContext() const {
#if ENABLE_ACCELERATION
    if (m_data->m_surface) {
        CGContextFlush(m_data->m_surface->ensurePlatformContext());
    }
#endif
}


scoped_refptr<ImageData> ImageBuffer::getUnmultipliedImageData(const IntRect &rect, IntSize *pixelArrayDimensions, ImageBuffer::CoordinateSystem coordinateSystem) const {
    IntRect srcRect = rect;
    if (coordinateSystem == LogicalCoordinateSystem)
        srcRect.scale(m_resolutionScale);

    if (pixelArrayDimensions)
        *pixelArrayDimensions = srcRect.size;

    std::unique_ptr<ImageBufferData> subBufferData = m_data->getData(AlphaPremultiplication::Unpremultiplied, srcRect, m_imageSize);

    auto result = ImageData::Create(subBufferData->bufferSize(), subBufferData->m_bytesPerRow, subBufferData->data(), false);
    ImageBufferData *bufferData = subBufferData.get();
    bufferData->m_buffer = nullptr;

    return result;
}

void ImageBuffer::putByteArray(const ImageData &source, const IntRect &sourceRect, const IntPoint &destPoint, ImageBuffer::CoordinateSystem coordinateSystem) {
    IntRect scaledSourceRect = sourceRect;
    IntSize scaledSourceSize = source.size();
    if (coordinateSystem == LogicalCoordinateSystem) {
        scaledSourceRect.scale(m_resolutionScale);
        scaledSourceSize.scale(m_resolutionScale);
    }

    ImageBufferData bufferData(scaledSourceSize);
    m_data->putData(source.data(), AlphaPremultiplication::Unpremultiplied, scaledSourceSize, scaledSourceRect, destPoint);
}

static CGColorSpaceRef colorSpace(const ImageBitmapConfiguration &configuration) {
    return configuration.colorSpace.get() ? configuration.colorSpace.get() : sRGBColorSpaceRef();
}

std::unique_ptr<GraphicsContextCG> ImageBuffer::createGraphicsContext() {
    std::unique_ptr<GraphicsContextCG> ctx;
    if (m_accelerationHint == kPreferNoAcceleration) {
        retain();
        IntSize scaledSize = m_imageSize;
        scaledSize.scale(m_resolutionScale);
        auto bitmapContext = adoptCF(CGBitmapContextCreateWithData(
                m_data->data(),
                static_cast<size_t>(scaledSize.width),
                static_cast<size_t>(scaledSize.height),
                m_bytesPerPixel * 8 / 4,
                scaledSize.width * m_bytesPerPixel,
                colorSpace(m_configuration),
                m_configuration.bitmapInfo,
                releaseBitmapContextData,
                this));

        CGContextTranslateCTM(bitmapContext.get(), 0, m_imageSize.height);
        CGContextScaleCTM(bitmapContext.get(), 1, -1);

        ctx = std::unique_ptr<GraphicsContextCG>(new GraphicsContextCG(bitmapContext));
    } else {
#if ENABLE_ACCELERATION
        ctx = std::unique_ptr<GraphicsContextCG>(new GraphicsContextCG(m_data->m_surface->ensurePlatformContext()));
#endif
    }

    return ctx;

}

RetainPtr<CGImageRef> ImageBuffer::makeCGImage() {

    IntSize scaledSize = m_imageSize;
    scaledSize.scale(m_resolutionScale);
    size_t bytesCount = scaledSize.width * scaledSize.height * m_bytesPerPixel;

    RetainPtr<CGImageRef> image;
    if (m_accelerationHint == kPreferNoAcceleration) {
        retain();
        CGDataProviderRef dataProvider = CGDataProviderCreateWithData(this, m_data->data(), bytesCount, releaseDataProviderData);

        image = adoptCF(CGImageCreate(
                static_cast<size_t>(scaledSize.width),
                static_cast<size_t>(scaledSize.height),
                m_bytesPerPixel * 8 / 4,
                m_bytesPerPixel * 8,
                scaledSize.width * m_bytesPerPixel,
                colorSpace(m_configuration),
                m_configuration.bitmapInfo,
                dataProvider,
                0,
                false,
                kCGRenderingIntentDefault));

        CGDataProviderRelease(dataProvider);
    } else {
#if ENABLE_ACCELERATION
        image = m_data->m_surface->createImage();
#endif
    }

    return image;
}

scoped_refptr<Image> ImageBuffer::copyNativeImage(BackingStoreCopy copyBehavior) {
    scoped_refptr<Image> image;
    if (m_accelerationHint == kPreferNoAcceleration) {
        RetainPtr<CGImageRef> cgImage;
        switch (copyBehavior) {
            case DontCopyBackingStore:
                cgImage = makeCGImage();
                break;
            case CopyBackingStore:
                cgImage = CGBitmapContextCreateImage(createGraphicsContext().get()->getCGContextRef());
                break;
            default:
                break;
        }

        image = MakeRefCounted<Image>(cgImage);
    } else {
#if ENABLE_ACCELERATION
        RetainPtr<CGImageRef> cgImage;
        cgImage = m_data->m_surface->createImage();
        image = MakeRefCounted<Image>(cgImage);
#endif
    }

    return image;
}

AffineTransform ImageBuffer::baseTransform() const {
    return AffineTransform(1, 0, 0, -1, 0, m_imageSize.height);
}

static FloatSize scaleSizeToUserSpace(const FloatSize &logicalSize, const IntSize &backingStoreSize, const IntSize &internalSize) {
    float xMagnification = static_cast<float>(backingStoreSize.width) / internalSize.width;
    float yMagnification = static_cast<float>(backingStoreSize.height) / internalSize.height;
    return FloatSize(logicalSize.width * xMagnification, logicalSize.height * yMagnification);
}


FloatSize ImageBuffer::sizeForDestinationSize(FloatSize destinationSize) const {
    return scaleSizeToUserSpace(destinationSize, m_data->bufferSize(), size());
}

void ImageBuffer::retain() {
    std::lock_guard<std::mutex> lock(m_lock);
    ref();
}

void ImageBuffer::release() {
    std::lock_guard<std::mutex> lock(m_lock);
    deref();
}

void ImageBuffer::releaseBitmapContextData(void *typelessBitmap, void *) {
    ImageBuffer *imageBuffer = static_cast<ImageBuffer *>(typelessBitmap);
    imageBuffer->release();
}

void ImageBuffer::releaseDataProviderData(void *typelessBitmap, const void *, size_t) {
    ImageBuffer *imageBuffer = static_cast<ImageBuffer *>(typelessBitmap);
    imageBuffer->release();
}

}