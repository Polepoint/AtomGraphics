//
// Created by neo on 2019-08-28.
//

#include "graphics/font/PaintTextBlobBuilder.h"


namespace AtomGraphics {

PaintTextBlobBuilder::PaintTextBlobBuilder() {

}

PaintTextBlobBuilder::~PaintTextBlobBuilder() {

}

scoped_refptr<PaintTextBlob> PaintTextBlobBuilder::TakeTextBlob() {
    auto result = MakeRefCounted<PaintTextBlob>(builder_.make(), std::move(typefaces_));
    typefaces_.clear();
    return result;
}

const PaintTextBlobBuilder::RunBuffer &PaintTextBlobBuilder::AllocRunPos(CTFontRef font, const PaintTypeface &typeface, int count) {
    return builder_.allocRunPos(font, count);
}

}