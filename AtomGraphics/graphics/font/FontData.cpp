//
// Created by neo on 2019-07-17.
//

#include "FontData.h"

namespace AtomGraphics {

FontData::FontData(
        const PaintTypeface &paint_tf,
        const std::string &family,
        float text_size,
        size_t weight,
        Slant slant)
        : paint_typeface_(paint_tf),
          family_(family),
          text_size_(text_size),
          weight_(weight),
          slant_(slant) {
    platformInit();
}

}