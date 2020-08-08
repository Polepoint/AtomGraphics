//
// Created by neo on 2019-07-09.
//

#include <skia/core/SkPaint.h>
#include "graphics/font/WordShaper.h"

namespace AtomGraphics {

scoped_refptr<const ShapeResult> WordShaper::shapeSingleWord(char *ch, ShapeResult *result, const FontData *font_data) {
    SkGlyphID glyph;
    font_data->paintTypeface().ToSkTypeface()->charsToGlyphs(ch, SkTypeface::kUTF8_Encoding, &glyph, 1);
    if (glyph) {
        result->glyph_ = glyph;
        result->primary_font_ = font_data;
        SkPaint paint;
        font_data->SetupPaintFont(&paint);
        result->advance_ = SkScalarToFloat(paint.measureText(ch, strlen(ch)));
        return scoped_refptr<const ShapeResult>(result);
    }

    return nullptr;
}


}