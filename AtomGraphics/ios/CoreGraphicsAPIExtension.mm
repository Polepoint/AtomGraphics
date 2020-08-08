//
// Created by neo on 2020/2/3.
// Copyright (c) 2020 neo. All rights reserved.
//

#import "CoreGraphicsAPIExtension.h"

#if !USE_CORE_GRAPHICS_SPI

#import "math/FloatRect.h"
#import "graphics/ios/CG/GraphicsContextCG.h"

#endif

namespace AtomGraphics {

#if USE_CORE_GRAPHICS_SPI

extern "C" {

void CGContextSetCTM(CGContextRef, CGAffineTransform);

void CGContextDrawPathDirect(CGContextRef, CGPathDrawingMode, CGPathRef, const CGRect *boundingBox);

CGPatternRef CGPatternCreateWithImage2(CGImageRef, CGAffineTransform, CGPatternTiling);

#if ENABLE_ACCELERATION

#import <IOSurface/IOSurfaceRef.h>

CGContextRef CGIOSurfaceContextCreate(IOSurfaceRef, size_t, size_t, size_t, size_t, CGColorSpaceRef, CGBitmapInfo);

CGImageRef CGIOSurfaceContextCreateImage(CGContextRef);

#endif

};

void ContextSetCTM(CGContextRef context, CGAffineTransform transform) {
    CGContextSetCTM(context, transform);
}

void ContextDrawPath(CGContextRef context, CGPathDrawingMode drawingMode, CGPathRef path, const CGRect *boundingBox) {
    CGContextDrawPathDirect(context, drawingMode, path, boundingBox);
}

CGPatternRef PatternCreateWithImage(CGImageRef img, CGAffineTransform transform, CGPatternTiling tiling) {
    return CGPatternCreateWithImage2(img, transform, tiling);
}

#else

void ContextSetCTM(CGContextRef context, CGAffineTransform transform) {
    CGAffineTransform currentCTM = CGContextGetCTM(context);
    CGAffineTransform invertedCTM = CGAffineTransformInvert(currentCTM);
    CGContextConcatCTM(context, invertedCTM);
    CGContextConcatCTM(context, transform);
}

void ContextDrawPath(CGContextRef context, CGPathDrawingMode drawingMode, CGPathRef path, const CGRect *boundingBox) {
    CGContextBeginPath(context);
    CGContextAddPath(context, path);
    CGContextDrawPath(context, drawingMode);
}


static void patternCallback(void *info, CGContextRef context) {
    CGImageRef platformImage = static_cast<CGImageRef>(info);
    if (!platformImage)
        return;

    CGRect rect = GraphicsContextCG(context).roundToDevicePixels(
            FloatRect(0, 0, CGImageGetWidth(platformImage), CGImageGetHeight(platformImage)));
    CGContextDrawImage(context, rect, platformImage);
}

static void patternReleaseCallback(void *info) {
    //TODO: release implementation
//    callOnMainThread([image = static_cast<CGImageRef>(info)] {
//        CGImageRelease(image);
//    });
}


static const CGPatternCallbacks patternCallbacks = {0, patternCallback, patternReleaseCallback};

CGPatternRef PatternCreateWithImage(CGImageRef img, CGAffineTransform transform, CGPatternTiling tiling) {
    if (img) {
        bool isMask = CGImageIsMask(img);
        CGFloat imageWidth = CGImageGetWidth(img);
        CGFloat imageHeight = CGImageGetHeight(img);
        CGRect bounds = CGRectMake(0, 0, imageWidth, imageHeight);
        return CGPatternCreate(img, bounds, transform, imageWidth, imageHeight, tiling, !isMask, &patternCallbacks);
    } else {
        return CGPatternCreate(img, CGRectZero, transform, 0, 0, tiling, true, &patternCallbacks);
    }
}

#endif

}
