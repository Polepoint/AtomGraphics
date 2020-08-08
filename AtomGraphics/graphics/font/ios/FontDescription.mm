//
// Created by neo on 2018/12/16.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "graphics/font/FontDescription.h"
#include "ios/CoreTextAPIExtension.h"

namespace AtomGraphics {

RetainPtr<CTFontDescriptorRef> FontDescription::ctFontDescriptor() const {


    auto attributes = adoptCF(CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks));

    CGFloat ctWeight = kFontWeightRegular;
    if (weight_ < 150)
        ctWeight = kFontWeightUltraLight;
    else if (weight_ < 250)
        ctWeight = kFontWeightThin;
    else if (weight_ < 350)
        ctWeight = kFontWeightLight;
    else if (weight_ < 450)
        ctWeight = kFontWeightRegular;
    else if (weight_ < 550)
        ctWeight = kFontWeightMedium;
    else if (weight_ < 650)
        ctWeight = kFontWeightSemibold;
    else if (weight_ < 750)
        ctWeight = kFontWeightBold;
    else if (weight_ < 850)
        ctWeight = kFontWeightHeavy;
    else
        ctWeight = kFontWeightBlack;

    auto weightNumberValue = adoptCF(CFNumberCreate(kCFAllocatorDefault, kCFNumberCGFloatType, &ctWeight));

    CFDictionaryAddValue(attributes.get(), kCTFontWeightTrait, weightNumberValue.get());

    if (slant_ == kItalic) {
        int trait = kCTFontItalicTrait;
        auto valueNumber = adoptCF(CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &trait));
        CFDictionaryAddValue(attributes.get(), kCTFontSymbolicTrait, valueNumber.get());
    } else if (slant_ == kOblique) {
        float slant = 0.07f;
        auto valueNumber = adoptCF(CFNumberCreate(kCFAllocatorDefault, kCFNumberFloatType, &slant));
        CFDictionaryAddValue(attributes.get(), kCTFontSlantTrait, valueNumber.get());
    }

    return adoptCF(CTFontDescriptorCreateWithAttributes(attributes.get()));
}

}