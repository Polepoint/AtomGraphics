//
// Created by neo on 2019/1/22.
//

#include "math/IntSize.h"

namespace AtomGraphics {

IntSize::operator SkISize() const {
    return SkISize::Make(width, height);
}


}