//
// Created by neo on 2020/02/05.
// Copyright (c) 2020 neo. All rights reserved.
//

#import "CoreTextAPIExtension.h"
#import "memory/ios/RetainPtr.h"

namespace AtomGraphics {


#if USE_CORE_Text_SPI

extern "C" CTFontRef CTFontCreateForCharactersWithLanguage(CTFontRef currentFont, const UTF16Char *characters, CFIndex length, CFStringRef language, CFIndex *coveredLength);

CTFontRef FontCreateForCharacterWithLanguage(CTFontRef currentFont, UTF16Char character, CFStringRef language) {
    CFIndex coveredLength;
    return CTFontCreateForCharactersWithLanguage(currentFont, &character, 1, language, &coveredLength);
}

#else

#if defined(__IPHONE_OS_VERSION_MAX_ALLOWED) && __IPHONE_OS_VERSION_MAX_ALLOWED < 130000

extern "C" CTFontRef CTFontCreateForStringWithLanguage(CTFontRef, CFStringRef, CFRange, CFStringRef);

#endif

CTFontRef FontCreateForCharacterWithLanguage(CTFontRef currentFont, UTF16Char character, CFStringRef language) {
    auto string = adoptCF(CFStringCreateWithCharactersNoCopy(kCFAllocatorDefault, &character, 1, kCFAllocatorNull));
    return CTFontCreateForStringWithLanguage(currentFont, string.get(), CFRangeMake(0, 1), language);
}

#endif

}