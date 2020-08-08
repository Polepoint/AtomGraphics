//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "FloatSize.h"
#include "base/AtomMacros.h"
#include "IntSize.h"

namespace AtomGraphics {

FloatSize::FloatSize(void) : width(0), height(0) {
}

FloatSize::FloatSize(float w, float h) : width(w), height(h) {
}

FloatSize::FloatSize(const FloatSize &other) : width(other.width), height(other.height) {
}

FloatSize::FloatSize(const IntSize &intSize) : width(intSize.width), height(intSize.height) {

}

FloatSize::FloatSize(const Vec2 &point) : width(point.x), height(point.y) {
}

FloatSize &FloatSize::operator=(const FloatSize &other) {
    setSize(other.width, other.height);
    return *this;
}

FloatSize &FloatSize::operator=(const Vec2 &point) {
    setSize(point.x, point.y);
    return *this;
}

FloatSize FloatSize::operator+(const FloatSize &right) const {
    return FloatSize(this->width + right.width, this->height + right.height);
}

void FloatSize::operator+=(const FloatSize &v) {
    width += v.width;
    height += v.height;
}

FloatSize FloatSize::operator-(const FloatSize &right) const {
    return FloatSize(this->width - right.width, this->height - right.height);
}

FloatSize FloatSize::operator*(float a) const {
    return FloatSize(this->width * a, this->height * a);
}

FloatSize FloatSize::operator/(float a) const {
    return FloatSize(this->width / a, this->height / a);
}

void FloatSize::setSize(float w, float h) {
    this->width = w;
    this->height = h;
}

bool FloatSize::equals(const FloatSize &target) const {
    return (std::abs(this->width - target.width) < FLT_EPSILON)
            && (std::abs(this->height - target.height) < FLT_EPSILON);
}

void FloatSize::scale(float scale) {
    width *= scale;
    height *= scale;
}

void FloatSize::scale(float scaleX, float scaleY) {
    width *= scaleX;
    height *= scaleY;
}

const FloatSize FloatSize::ZERO = FloatSize(0, 0);

}