//
// Created by neo on 2019-07-11.
//

#include "FontFamily.h"


namespace AtomGraphics {

FontFamily::FontFamily(const FontFamily &other) {
    family_ = other.family_;
    next_ = other.next_;
}

}