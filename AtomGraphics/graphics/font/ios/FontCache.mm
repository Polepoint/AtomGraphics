//
// Created by neo on 2018/12/16.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "graphics/font/FontCache.h"
#import "SystemFontCache.h"
#import "CoreTextAPIExtension.h"

namespace AtomGraphics {

static SystemFontCache *SharedSystemFontCache() {
    static SystemFontCache *fontCache = new SystemFontCache();
    return fontCache;
}


scoped_refptr <FontData> FontCache::FallbackFontForCharacter(const Font &font, UChar32 lookup_char) {

    std::string family_name = GetFamilyNameForCharacter(lookup_char, font.PrimaryFontData());
    return GetFontData(font.fontDescription(), family_name);
}

std::string FontCache::GetFamilyNameForCharacter(UChar32 c, const FontData *primaryFont) {
    RetainPtr<CFStringRef> family;
    UChar character = static_cast<UChar>(c);
    SharedSystemFontCache()->get([&family, character](const SystemFontCacheEntry &cacheEntry) -> bool {
        auto &font = cacheEntry.m_font;
        CGGlyph glyph;
        CTFontGetGlyphsForCharacters(cacheEntry.m_font.get(), &character, &glyph, 1);
        if (glyph) {
            family = CTFontCopyFamilyName(font.get());
            return true;
        }

        return false;
    });

    if (!family) {
        static NSString *localeLanguageCode = [[NSLocale currentLocale] objectForKey:NSLocaleLanguageCode];
        RetainPtr<CTFontRef> systemFont = adoptCF(FontCreateForCharacterWithLanguage(
                primaryFont->paintTypeface().CTFont().get(),
                character, (__bridge CFStringRef) localeLanguageCode));
        family = CTFontCopyFamilyName(systemFont.get());
        std::string name = std::string(CFStringGetCStringPtr(family.get(), kCFStringEncodingUTF8));
        SharedSystemFontCache()->add(SystemFontCacheEntry(name, systemFont));
    }

    return std::string(CFStringGetCStringPtr(family.get(), kCFStringEncodingUTF8));
}


scoped_refptr<FontData> FontCache::GetLastResortFallbackFont(const FontDescription &description) {
    scoped_refptr<FontData> result = GetFontData(description, "Times");

    if (!result) {
        result = GetFontData(description, "PingFang SC");
    }

    return result;
}

}