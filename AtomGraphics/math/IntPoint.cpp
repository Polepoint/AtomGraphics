//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "IntPoint.h"

namespace AtomGraphics {

IntPoint::IntPoint() : x(0), y(0) {

}

IntPoint::IntPoint(int _x, int _y) : x(_x), y(_y) {

}

IntPoint::IntPoint(const IntPoint &other) {
    x = other.x;
    y = other.y;
}

IntPoint::IntPoint(const IntSize &size) {
    x = size.width;
    y = size.height;
}

IntPoint &IntPoint::operator=(const IntPoint &other) {
    x = other.x;
    y = other.y;
    return *this;
}

}