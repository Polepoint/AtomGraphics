//
// Created by neo on 2018/11/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "math/FloatPoint.h"

namespace AtomGraphics {

FloatPoint::FloatPoint(const CGPoint &point) {
    x = point.x;
    y = point.y;
}

FloatPoint::operator CGPoint() const {
    return CGPointMake(x, y);
}
    
}