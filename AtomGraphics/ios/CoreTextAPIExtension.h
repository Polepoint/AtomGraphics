//
// Created by neo on 2019-08-29.
//

#ifndef ATOMGRAPHICS_CORETEXTAPIEXTENSION_H
#define ATOMGRAPHICS_CORETEXTAPIEXTENSION_H

#import <CoreText/CoreText.h>

namespace AtomGraphics {

#if USE_CORE_Text_SPI

extern "C" {

extern const CGFloat kCTFontWeightUltraLight;
extern const CGFloat kCTFontWeightThin;
extern const CGFloat kCTFontWeightLight;
extern const CGFloat kCTFontWeightRegular;
extern const CGFloat kCTFontWeightMedium;
extern const CGFloat kCTFontWeightSemibold;
extern const CGFloat kCTFontWeightBold;
extern const CGFloat kCTFontWeightHeavy;
extern const CGFloat kCTFontWeightBlack;

}

const CGFloat kFontWeightUltraLight = kCTFontWeightUltraLight;
const CGFloat kFontWeightThin = kCTFontWeightThin;
const CGFloat kFontWeightLight = kCTFontWeightLight;
const CGFloat kFontWeightRegular = kCTFontWeightRegular;
const CGFloat kFontWeightMedium = kCTFontWeightMedium;
const CGFloat kFontWeightSemibold = kCTFontWeightSemibold;
const CGFloat kFontWeightBold = kCTFontWeightBold;
const CGFloat kFontWeightHeavy = kCTFontWeightHeavy;
const CGFloat kFontWeightBlack = kCTFontWeightBlack;

#else

#import <UIKit/UIFontDescriptor.h>

const CGFloat kFontWeightUltraLight = UIFontWeightUltraLight;
const CGFloat kFontWeightThin = UIFontWeightThin;
const CGFloat kFontWeightLight = UIFontWeightLight;
const CGFloat kFontWeightRegular = UIFontWeightRegular;
const CGFloat kFontWeightMedium = UIFontWeightMedium;
const CGFloat kFontWeightSemibold = UIFontWeightSemibold;
const CGFloat kFontWeightBold = UIFontWeightBold;
const CGFloat kFontWeightHeavy = UIFontWeightHeavy;
const CGFloat kFontWeightBlack = UIFontWeightBlack;

#endif

CTFontRef FontCreateForCharacterWithLanguage(CTFontRef currentFont, UTF16Char character, CFStringRef language);

}


#endif //ATOMGRAPHICS_CORETEXTAPIEXTENSION_H
