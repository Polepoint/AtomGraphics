//
// Created by neo on 2019/9/22.
//

#ifndef ATOMGRAPHICS_ATIOSURFACE_H
#define ATOMGRAPHICS_ATIOSURFACE_H

#if ENABLE_ACCELERATION

#import <IOSurface/IOSurfaceRef.h>
#import <CoreGraphics/CGContext.h>
#import "RetainPtr.h"
#import "IntSize.h"
#import "GraphicsContext.h"

namespace AtomGraphics {

class ATIOSurface final {

public:

    enum class Format {
        RGBA,
        YUV422,
        RGB10,
        RGB10A8,
    };


    class Locker {
    public:
        enum class AccessMode {
            ReadOnly,
            ReadWrite
        };

        Locker(ATIOSurface &surface, AccessMode mode = AccessMode::ReadOnly)
                : m_surface(surface), m_flags(flagsFromMode(mode)) {
            IOSurfaceLock(m_surface.surface(), m_flags, nullptr);
        }

        ~Locker() {
            IOSurfaceUnlock(m_surface.surface(), m_flags, nullptr);
        }

        void *surfaceBaseAddress() const {
            return IOSurfaceGetBaseAddress(m_surface.surface());
        }

    private:
        static uint32_t flagsFromMode(AccessMode mode) {
            return mode == AccessMode::ReadOnly ? kIOSurfaceLockReadOnly : 0;
        }

        ATIOSurface &m_surface;
        uint32_t m_flags;
    };


//    static std::unique_ptr<ATIOSurface> create(IntSize, CGColorSpaceRef, Format = Format::RGBA);

    static std::unique_ptr<ATIOSurface> create(IntSize, IntSize contextSize, CGColorSpaceRef, Format = Format::RGBA);

//    static std::unique_ptr<ATIOSurface> createFromSendRight(const WTF::MachSendRight &&, CGColorSpaceRef);

//    static std::unique_ptr<ATIOSurface> createFromSurface(IOSurfaceRef, CGColorSpaceRef);

//    static std::unique_ptr<ATIOSurface> createFromImage(CGImageRef);

//    static std::unique_ptr<ATIOSurface> createFromImageBuffer(std::unique_ptr<ImageBuffer>);

    static void moveToPool(std::unique_ptr<ATIOSurface> &&);

//    static IntSize maximumSize();

//    WTF::MachSendRight createSendRight() const;

    // Any images created from a surface need to be released before releasing
    // the surface, or an expensive GPU readback can result.
    RetainPtr<CGImageRef> createImage();

//    static RetainPtr<CGImageRef> sinkIntoImage(std::unique_ptr<ATIOSurface>);

#ifdef __OBJC__

    id asLayerContents() const { return (__bridge id) m_surface.get(); }

#endif

    IOSurfaceRef surface() const { return m_surface.get(); }

//    GraphicsContext &ensureGraphicsContext();

    CGContextRef ensurePlatformContext();

    enum class SurfaceState {
        Valid,
        Empty
    };

//    // Querying volatility can be expensive, so in cases where the surface is
//    // going to be used immediately, use the return value of setIsVolatile to
//    // determine whether the data was purged, instead of first calling state() or isVolatile().
//    SurfaceState state() const;
//
//    bool isVolatile() const;
//
    // setIsVolatile only has an effect on iOS and OS 10.9 and above.
    SurfaceState setIsVolatile(bool);

    IntSize size() const { return m_size; }

    size_t totalBytes() const { return m_totalBytes; }

    CGColorSpaceRef colorSpace() const { return m_colorSpace.get(); }

    Format format() const;

//    IOSurfaceID surfaceID() const;
//
//    size_t bytesPerRow() const;

    bool isInUse() const;

    // The graphics context cached on the surface counts as a "user", so to get
    // an accurate result from isInUse(), it needs to be released.
    void releaseGraphicsContext();
//
//    static bool allowConversionFromFormatToFormat(Format, Format);
//
//    static void convertToFormat(std::unique_ptr<ATIOSurface> &&inSurface, Format, std::function<void(std::unique_ptr<ATIOSurface>)> &&);
//
//    void migrateColorSpaceToProperties();

private:

    ATIOSurface(IntSize, IntSize contextSize, CGColorSpaceRef, Format, bool &success);

//    ATIOSurface(IOSurfaceRef, CGColorSpaceRef);

    static std::unique_ptr<ATIOSurface> surfaceFromPool(IntSize, IntSize contextSize, CGColorSpaceRef, Format);

    IntSize contextSize() const { return m_contextSize; }

    void setContextSize(IntSize);

    RetainPtr<CGColorSpaceRef> m_colorSpace;
    IntSize m_size;
    IntSize m_contextSize;
    size_t m_totalBytes;

    RetainPtr<CGContextRef> m_cgContext;

    RetainPtr<IOSurfaceRef> m_surface;
};

};

#endif


#endif //ATOMGRAPHICS_ATIOSURFACE_H
