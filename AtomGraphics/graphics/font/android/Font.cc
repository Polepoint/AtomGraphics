//
// Created by neo on 2018/8/29.
//

#include <vector>
#include "graphics/font/Font.h"
#include "graphics/font/FontCache.h"
#include "graphics/font/FontFamily.h"
#include "graphics/font/WordShaper.h"
#include "graphics/font/ShapeResultBloberizer.h"


namespace AtomGraphics {

//sk_sp<SkTextBlob> Font::blobsForString(const base::String *str) {
//
//    return sk_sp<SkTextBlob>();
//}

void Font::drawBidiText(
        SkCanvas *canvas,
        const base::String &text,
        const FloatPoint &position,
        const SkPaint &paint) const {

    WordShaper shaper(*this);
    ShapeResultBloberizer bloberizer;
    const UChar *c = text.toUCharData();
    float offset = 0;
    for (int i = 0; i < text.length(); i++) {
        scoped_refptr<const ShapeResult> result = shaper.shapeCharacter(c[i]);
        bloberizer.add(result->glyph_, result->primary_font_.get(), offset);
        offset += result->advance_;
    }
    const std::vector<ShapeResultBloberizer::BlobInfo> blobs = bloberizer.Blobs();

    for (auto &blob : blobs) {
        canvas->drawTextBlob(blob.blob->ToSkTextBlob(), position.x, position.y, paint);
    }
}

}