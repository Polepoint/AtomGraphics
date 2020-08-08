//
// Created by neo on 2018/11/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "math/FloatPoint.h"

namespace AtomGraphics {

FloatPoint::FloatPoint(const SkPoint &point) {
    x = SkScalarToFloat(point.x());
    y = SkScalarToFloat(point.y());
}

FloatPoint::operator SkPoint() const {
    return SkPoint::Make(SkFloatToScalar(x), SkFloatToScalar(y));
}

}