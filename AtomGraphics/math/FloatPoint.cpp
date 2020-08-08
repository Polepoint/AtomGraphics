//
// Created by neo on 2018/11/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "FloatPoint.h"
#include "IntPoint.h"

namespace AtomGraphics {

FloatPoint::FloatPoint(const IntPoint &intPoint) : Vec2(intPoint.x, intPoint.y) {}

FloatPoint::FloatPoint(const FloatPoint &other) : Vec2(other.x, other.y) {}

FloatPoint::FloatPoint() : Vec2() {}

FloatPoint::FloatPoint(float xx, float yy) : Vec2(xx, yy) {}

FloatPoint::FloatPoint(const float *array) : Vec2(array) {}

FloatPoint::FloatPoint(const Vec2 &p1, const Vec2 &p2) : Vec2(p1, p2) {}

FloatPoint::FloatPoint(const Vec2 &copy) : Vec2(copy) {}

FloatPoint::~FloatPoint() {}

void FloatPoint::move(float dx, float dy) {
    x += dx;
    y += dy;
}

IntPoint roundedIntPoint(const FloatPoint &p) {
    return IntPoint(static_cast<int>(roundf(p.x)), static_cast<int>(roundf(p.y)));
}

}