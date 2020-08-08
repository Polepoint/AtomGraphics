//
// Created by neo on 2019-07-19.
//

#include "graphics/font/PaintTextBlob.h"

namespace AtomGraphics {

PaintTextBlob::PaintTextBlob(scoped_refptr<TextBlob> blob, std::vector<PaintTypeface> typefaces)
        : blob_(std::move(blob)), typefaces_(std::move(typefaces)) {
}


PaintTextBlob::operator bool() const {
    return !!blob_;
}

}