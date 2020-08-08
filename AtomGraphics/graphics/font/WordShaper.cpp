//
// Created by neo on 2019-07-09.
//

#include "FontCache.h"
#include "WordShaper.h"

namespace AtomGraphics {

WordShaper::WordShaper(const Font &font) : font_(font) {

}

scoped_refptr<const ShapeResult> WordShaper::shapeCharacter(UChar c) {
    ShapeCache *cache = font_.getShapeCache(font_.fontDescription());
    ShapeCacheEntry *entry = cache->add(c, ShapeCacheEntry());
    if (entry->shape_result_) {
        return entry->shape_result_;
    } else {
        entry->shape_result_ = shape(c);
    }

    return entry->shape_result_;
}

scoped_refptr<const ShapeResult> WordShaper::shape(UChar unicode) {
    const FontDescription &description = font_.fontDescription();
    const FontFallbackList &fallbackList = font_.fallbackList();

    char ch[5] = {'\0'};
    base::convertUTF16ToUTF8(unicode, ch);

    ShapeResult *result = new ShapeResult();
    result->unicode_ = unicode;

    for (int font_index = 0;; font_index++) {
        const FontData *font_data = fallbackList.fontDataAt(description, font_index);
        if (font_data) {
#if PLATFORM(ANDROID)
            scoped_refptr<const ShapeResult> shapeResult = shapeSingleWord(ch, result, font_data);
#elif PLATFORM(IOS)
            scoped_refptr<const ShapeResult> shapeResult = shapeSingleWord(unicode, result, font_data);
#endif
            if (shapeResult) {
                return shapeResult;
            }
        } else {
            break;
        }
    }

    return shapeWithSystemFonts(font_, result, unicode, ch);
}

scoped_refptr<const ShapeResult> WordShaper::shapeWithSystemFonts(
        const Font &font,
        ShapeResult *result,
        UChar unicode,
        char *c) {

#if PLATFORM(ANDROID)
    scoped_refptr<FontData> system_font = FontCache::GetFontCache()->FallbackFontForCharacter(
            font.fontDescription(), unicode);
    if (system_font) {
        return shapeSingleWord(c, result, system_font.get());
    } else {
        return nullptr;
    }
#elif PLATFORM(IOS)
    scoped_refptr<FontData> system_font = FontCache::GetFontCache()->FallbackFontForCharacter(font, unicode);
    if (system_font) {
        return shapeSingleWord(unicode, result, system_font.get());
    } else {
        return nullptr;
    }
#endif
}


}