//
// Created by neo on 2019-07-19.
//

#include "graphics/font/PaintTextBlob.h"

namespace AtomGraphics {

PaintTextBlob::PaintTextBlob(sk_sp<SkTextBlob> blob,
                             std::vector<PaintTypeface> typefaces)
        : sk_blob_(std::move(blob)), typefaces_(std::move(typefaces)) {}


PaintTextBlob::operator bool() const {
    return !!sk_blob_;
}

}