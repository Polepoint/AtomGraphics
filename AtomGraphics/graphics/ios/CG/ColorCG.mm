//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <mutex>
#import "ColorCG.h"
#import "cache/TinyLRUCache.h"
#import "RetainPtr.h"

namespace AtomGraphics {

CGColorSpaceRef sRGBColorSpaceRef() {
    static CGColorSpaceRef sRGBColorSpace;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        sRGBColorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    });
    return sRGBColorSpace;
}

static CGColorRef leakCGColor(const Color4F &color) {
    CGFloat components[4] = {color.r, color.g, color.b, color.a};
    return CGColorCreate(sRGBColorSpaceRef(), components);
}

template<>
RetainPtr<CGColorRef> TinyLRUCachePolicy<Color4F, RetainPtr<CGColorRef>>::createValueForKey(const Color4F &color) {
    return adoptCF(leakCGColor(color));
}

CGColorRef cachedCGColor(const Color4F &color) {
    static TinyLRUCache<Color4F, RetainPtr<CGColorRef>, 32> cache;
    return cache.get(color).get();
}

}