//
// Created by neo on 2019/9/22.
//

#if ENABLE_ACCELERATION

#import "ATIOSurface.h"
#import "ATIOSurfacePool.h"
#import "CoreGraphicsSPI.h"

namespace AtomGraphics {


inline std::unique_ptr<ATIOSurface> ATIOSurface::surfaceFromPool(IntSize size, IntSize contextSize, CGColorSpaceRef colorSpace, Format pixelFormat) {
    auto cachedSurface = ATIOSurfacePool::sharedPool().takeSurface(size, colorSpace, pixelFormat);
    if (!cachedSurface)
        return nullptr;

    cachedSurface->setContextSize(contextSize);
    return cachedSurface;
}


std::unique_ptr<ATIOSurface> ATIOSurface::create(IntSize size, IntSize contextSize, CGColorSpaceRef colorSpace, ATIOSurface::Format pixelFormat) {
    if (auto cachedSurface = surfaceFromPool(size, contextSize, colorSpace, pixelFormat)) {
        return cachedSurface;
    }

    bool success = false;
    auto surface = std::unique_ptr<ATIOSurface>(new ATIOSurface(size, contextSize, colorSpace, pixelFormat, success));
    if (!success) {
        return nullptr;
    }

    return surface;
}

static NSDictionary *optionsFor32BitSurface(IntSize size, unsigned pixelFormat) {
    int width = size.width;
    int height = size.height;

    unsigned bytesPerElement = 4;
    unsigned bytesPerPixel = 4;

    size_t bytesPerRow = IOSurfaceAlignProperty(kIOSurfaceBytesPerRow, width * bytesPerPixel);

    size_t totalBytes = IOSurfaceAlignProperty(kIOSurfaceAllocSize, height * bytesPerRow);

    return @{
            (id) kIOSurfaceWidth: @(width),
            (id) kIOSurfaceHeight: @(height),
            (id) kIOSurfacePixelFormat: @(pixelFormat),
            (id) kIOSurfaceBytesPerElement: @(bytesPerElement),
            (id) kIOSurfaceBytesPerRow: @(bytesPerRow),
            (id) kIOSurfaceAllocSize: @(totalBytes),
            (id) kIOSurfaceCacheMode: @(kIOSurfaceMapWriteCombineCache),
            (id) kIOSurfaceElementHeight: @(1)
    };

}

static NSDictionary *optionsForBiplanarSurface(IntSize size, unsigned pixelFormat, size_t firstPlaneBytesPerPixel, size_t secondPlaneBytesPerPixel) {
    int width = size.width;
    int height = size.height;

    size_t firstPlaneBytesPerRow = IOSurfaceAlignProperty(kIOSurfaceBytesPerRow, width * firstPlaneBytesPerPixel);
    size_t firstPlaneTotalBytes = IOSurfaceAlignProperty(kIOSurfaceAllocSize, height * firstPlaneBytesPerRow);

    size_t secondPlaneBytesPerRow = IOSurfaceAlignProperty(kIOSurfaceBytesPerRow, width * secondPlaneBytesPerPixel);
    size_t secondPlaneTotalBytes = IOSurfaceAlignProperty(kIOSurfaceAllocSize, height * secondPlaneBytesPerRow);

    size_t totalBytes = firstPlaneTotalBytes + secondPlaneTotalBytes;

    NSArray *planeInfo = @[
            @{
                    (id) kIOSurfacePlaneWidth: @(width),
                    (id) kIOSurfacePlaneHeight: @(height),
                    (id) kIOSurfacePlaneBytesPerRow: @(firstPlaneBytesPerRow),
                    (id) kIOSurfacePlaneOffset: @(0),
                    (id) kIOSurfacePlaneSize: @(firstPlaneTotalBytes)
            },
            @{
                    (id) kIOSurfacePlaneWidth: @(width),
                    (id) kIOSurfacePlaneHeight: @(height),
                    (id) kIOSurfacePlaneBytesPerRow: @(secondPlaneBytesPerRow),
                    (id) kIOSurfacePlaneOffset: @(firstPlaneTotalBytes),
                    (id) kIOSurfacePlaneSize: @(secondPlaneTotalBytes)
            }
    ];

    return @{
            (id) kIOSurfaceWidth: @(width),
            (id) kIOSurfaceHeight: @(height),
            (id) kIOSurfacePixelFormat: @(pixelFormat),
            (id) kIOSurfaceAllocSize: @(totalBytes),
            (id) kIOSurfaceCacheMode: @(kIOSurfaceMapWriteCombineCache),
            (id) kIOSurfacePlaneInfo: planeInfo,
    };
}

ATIOSurface::ATIOSurface(IntSize size, IntSize contextSize, CGColorSpaceRef colorSpace, Format format, bool &success)
        : m_colorSpace(colorSpace), m_size(size), m_contextSize(contextSize) {

    NSDictionary *options;

    switch (format) {
        case Format::RGBA:
            options = optionsFor32BitSurface(size, 'BGRA');
            break;
        case Format::RGB10:
            options = optionsFor32BitSurface(size, 'w30r');
            break;
        case Format::RGB10A8:
            options = optionsForBiplanarSurface(size, 'b3a8', 4, 1);
            break;
        case Format::YUV422:
            options = optionsForBiplanarSurface(size, '422f', 1, 1);
            break;
    }
    m_surface = adoptCF(IOSurfaceCreate((__bridge CFDictionaryRef) options));
    success = !!m_surface;
    if (success)
        m_totalBytes = IOSurfaceGetAllocSize(m_surface.get());
    else {
        //TODO: log error
    }
}

bool ATIOSurface::isInUse() const {
    return IOSurfaceIsInUse(m_surface.get());
}

void ATIOSurface::releaseGraphicsContext() {
    m_cgContext = nullptr;
}


void ATIOSurface::setContextSize(IntSize contextSize) {
    if (contextSize == m_contextSize)
        return;

    // Release the graphics context and update the context size. Next time the graphics context is
    // accessed, we will construct it again with the right size.
    releaseGraphicsContext();
    m_contextSize = contextSize;
}

ATIOSurface::SurfaceState ATIOSurface::setIsVolatile(bool isVolatile) {
    uint32_t previousState = 0;
    IOSurfaceSetPurgeable(m_surface.get(), isVolatile ? kIOSurfacePurgeableVolatile : kIOSurfacePurgeableNonVolatile, &previousState);

    if (previousState == kIOSurfacePurgeableEmpty)
        return ATIOSurface::SurfaceState::Empty;

    return ATIOSurface::SurfaceState::Valid;
}

ATIOSurface::Format ATIOSurface::format() const {
    unsigned pixelFormat = IOSurfaceGetPixelFormat(m_surface.get());
    if (pixelFormat == 'BGRA')
        return Format::RGBA;

    if (pixelFormat == 'w30r')
        return Format::RGB10;

    if (pixelFormat == 'b3a8')
        return Format::RGB10A8;

    if (pixelFormat == '422f')
        return Format::YUV422;

    return Format::RGBA;
}


CGContextRef ATIOSurface::ensurePlatformContext() {
    if (m_cgContext)
        return m_cgContext.get();

    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host;

    size_t bitsPerComponent = 8;
    size_t bitsPerPixel = 32;

    switch (format()) {
        case Format::RGBA:
            break;
        case Format::RGB10:
        case Format::RGB10A8:
            // A half-float format will be used if CG needs to read back the ATIOSurface contents,
            // but for an ATIOSurface-to-ATIOSurface copy, there should be no conversion.
            bitsPerComponent = 16;
            bitsPerPixel = 64;
            bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder16Host | kCGBitmapFloatComponents;
            break;
        case Format::YUV422:
            break;
    }

    m_cgContext = adoptCF(CGIOSurfaceContextCreate(m_surface.get(), m_contextSize.width, m_contextSize.height, bitsPerComponent, bitsPerPixel, m_colorSpace.get(), bitmapInfo));
    /**
     * alternative
     * https://developer.apple.com/library/archive/samplecode/DeepImageDisplayWithOpenGL/Listings/Sources_IOSurface2D_mm.html
     *
     * void* pBaseAddr = IOSurfaceGetBaseAddressOfPlane(m_surface.get(), 0);
     * if(pBaseAddr != nullptr)
     * {
     *      m_cgContext = adoptCF(CGBitmapContextCreate(pBaseAddr,
     *                                                  m_contextSize.width,
     *                                                  m_contextSize.height,
     *                                                  bitsPerComponent,
     *                                                  bitsPerPixel,
     *                                                  m_colorSpace.get(),
     *                                                  bitmapInfo));
     * }
     */

    return m_cgContext.get();
}

RetainPtr<CGImageRef> ATIOSurface::createImage() {
    /**
     * alternative
     * CIContext * ciContext = [CIContext contextWithCGContext:ensurePlatformContext() options:nil];
     * CGImageRef img = [ciContext createCGImage:xxx fromRect:CGRectMake(0, 0, m_contextSize.width, m_contextSize.height)];
     */
    return adoptCF(CGIOSurfaceContextCreateImage(ensurePlatformContext()));
}

void ATIOSurface::moveToPool(std::unique_ptr<ATIOSurface> &&surface) {
    ATIOSurfacePool::sharedPool().addSurface(std::move(surface));
}

}

#endif
