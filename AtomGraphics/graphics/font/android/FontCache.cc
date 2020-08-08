//
// Created by neo on 2019-07-11.
//


#include <skia/core/SkFontMgr.h>
#include "graphics/font/FontCache.h"


namespace AtomGraphics {

scoped_refptr<FontData> FontCache::FallbackFontForCharacter(
        const FontDescription &font_description,
        UChar32 lookup_char) {
    sk_sp<SkFontMgr> fm(SkFontMgr::RefDefault());
    std::string family_name = GetFamilyNameForCharacter(fm.get(), lookup_char);

    if (!family_name.size())
        return GetLastResortFallbackFont(font_description);
    return GetFontData(font_description, family_name);
}

std::string FontCache::GetFamilyNameForCharacter(SkFontMgr *fm, UChar32 c) {

    sk_sp<SkTypeface> typeface(fm->matchFamilyStyleCharacter(nullptr, SkFontStyle(), nullptr, 0, c));
    if (!typeface)
        return "";

    SkString skia_family_name;
    typeface->getFamilyName(&skia_family_name);
    return skia_family_name.c_str();
}

scoped_refptr<FontData> FontCache::GetLastResortFallbackFont(const FontDescription &description) {

    scoped_refptr<FontData> result = GetFontData(description, "Sans");

    if (!result) {
        result = GetFontData(description, "Arial");
    }

    return result;
}


}