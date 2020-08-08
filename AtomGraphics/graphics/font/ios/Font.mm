//
// Created by neo on 2018/8/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <CoreText/CoreText.h>
#import "Font.h"
#import "WordShaper.h"
#import "ShapeResultBloberizer.h"

namespace AtomGraphics {

void Font::drawBidiText(CGContextRef context, const base::String &text, const FloatPoint &position) const {
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

    CGAffineTransform transform = CGAffineTransformMakeTranslation(position.x, position.y);
    transform = CGAffineTransformScale(transform, 1.0f, -1.0f);
    CGContextSetTextMatrix(context, transform);

    for (auto blob : blobs) {
        auto &textBlob = blob.blob->ToTextBlob();
        CTFontDrawGlyphs(textBlob->font(), textBlob->glyphs(), textBlob->pos(), textBlob->glyphCount(), context);
    }
}


}
