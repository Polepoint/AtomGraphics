//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "Font.h"
#include "base/ATString.h"
#include "FontCache.h"
#include "WordShaper.h"

namespace AtomGraphics {


Font::Font() {}

Font::Font(const Font &other) {
    *this = other;
}

Font::Font(const FontDescription &fd) : font_description_(fd) {}

const FontFallbackList &Font::fallbackList() const {
    return m_fallbackList;
}

const std::string &Font::cachedCSSConfig() const {
    return m_cssConfig;
}

void Font::cacheCSSConfig(std::string &css) {
    m_cssConfig = css;
}

float Font::widthForString(const base::String &measuringString) const {
    unsigned length = measuringString.length();
    return widthForCharacters(measuringString.toUCharData(), length);
}

ShapeCache *Font::getShapeCache(const FontDescription &fontDescription) const {
    if (!shape_cache_) {
        FallbackListCompositeKey key = compositeKey(fontDescription);
        shape_cache_ = FontCache::GetFontCache()->GetShapeCache(key);
    }

    return shape_cache_;
}

FallbackListCompositeKey Font::compositeKey(const FontDescription &font_description) const {
    FallbackListCompositeKey key(font_description);
    const FontFamily *current_family = &font_description.Family();
    while (current_family) {
        if (current_family->Family().length()) {
            scoped_refptr<FontData> result = FontCache::GetFontCache()
                    ->GetFontData(font_description, current_family->Family());

            if (result) {
                key.add(FontCacheKey(0, 0, current_family->Family()));
            }
        }

        current_family = current_family->Next();
    }

    return key;
}

float Font::widthForCharacters(const UChar *characters, unsigned int length) const {
    WordShaper shaper(*this);

    float width = 0;
    for (int i = 0; i < length; i++) {
        scoped_refptr<const ShapeResult> result = shaper.shapeCharacter(characters[i]);
        width += result->getAdvance();
    }

    return width;
}


void Font::reset() {
    m_fontMetrics.reset();
    m_cssConfig.clear();

}

}