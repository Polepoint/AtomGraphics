//
// Created by neo on 2018/11/15.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "math/FloatRect.h"

namespace AtomGraphics {


FloatRect::FloatRect(const SkRect &skRect) {
    setRect(SkScalarToFloat(skRect.x()),
            SkScalarToFloat(skRect.y()),
            SkScalarToFloat(skRect.width()),
            SkScalarToFloat(skRect.height()));
}

FloatRect::operator SkRect() const {
    return SkRect::MakeXYWH(
            SkFloatToScalar(origin.x),
            SkFloatToScalar(origin.y),
            SkFloatToScalar(size.width),
            SkFloatToScalar(size.height));
}

}
