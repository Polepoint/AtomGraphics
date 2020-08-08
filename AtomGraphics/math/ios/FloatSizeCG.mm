//
// Created by neo on 2019/2/11.
//

#include "math/FloatSize.h"

namespace AtomGraphics {

FloatSize::operator CGSize() const {
    return CGSizeMake(width, height);
}

}
