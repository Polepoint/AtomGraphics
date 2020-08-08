//
// Created by neo on 2019-07-19.
//

#include "graphics/font/PaintTextBlobBuilder.h"

namespace AtomGraphics {

PaintTextBlobBuilder::PaintTextBlobBuilder() = default;

PaintTextBlobBuilder::~PaintTextBlobBuilder() = default;

scoped_refptr<PaintTextBlob> PaintTextBlobBuilder::TakeTextBlob() {
    auto result = MakeRefCounted<PaintTextBlob>(sk_builder_.make(), std::move(typefaces_));
    typefaces_.clear();
    return result;
}

const PaintTextBlobBuilder::RunBuffer &PaintTextBlobBuilder::AllocRunPosH(
        const SkPaint &font,
        const PaintTypeface &typeface,
        int count,
        SkScalar y,
        const SkRect *bounds) {
    typefaces_.push_back(typeface);
    return sk_builder_.allocRunPosH(font, count, y, bounds);
}

const PaintTextBlobBuilder::RunBuffer &PaintTextBlobBuilder::AllocRunPos(
        const SkPaint &font,
        const PaintTypeface &typeface,
        int count,
        const SkRect *bounds) {
    typefaces_.push_back(typeface);
    return sk_builder_.allocRunPos(font, count, bounds);
}

}