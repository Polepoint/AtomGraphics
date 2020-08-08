//
// Created by neo on 2020/5/5.
//

#ifndef ATOMGRAPHICS_IMAGEBITMAPCONFIGURATION_H
#define ATOMGRAPHICS_IMAGEBITMAPCONFIGURATION_H

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CoreGraphics.h>
#include "memory/ios/RetainPtr.h"

#endif

namespace AtomGraphics {

struct ImageBitmapConfiguration {
    bool isOpaque{false};
#if PLATFORM(IOS)
    RetainPtr <CGColorSpaceRef> colorSpace;
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host;
#endif

};

};


#endif //ATOMGRAPHICS_IMAGEBITMAPCONFIGURATION_H
