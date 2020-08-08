//
// Created by neo on 2019-07-09.
//

#include "graphics/font/WordShaper.h"

namespace AtomGraphics {

scoped_refptr<const ShapeResult> WordShaper::shapeSingleWord(UChar character, ShapeResult *result, const FontData *font_data) {
    CGGlyph glyph;
    CTFontGetGlyphsForCharacters(font_data->paintTypeface().CTFont().get(), &character, &glyph, 1);
    if (glyph) {
        result->glyph_ = glyph;
        result->primary_font_ = font_data;
        CGSize advance;
        CTFontGetAdvancesForGlyphs(font_data->paintTypeface().CTFont().get(), kCTFontOrientationHorizontal, &glyph, &advance, 1);
        result->advance_ = advance.width;
        return scoped_refptr<const ShapeResult>(result);
    }

    return nullptr;
}

}