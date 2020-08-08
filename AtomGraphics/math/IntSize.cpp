//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "IntSize.h"

namespace AtomGraphics {

IntSize::IntSize(void) : width(0), height(0) {}

IntSize::IntSize(size_t w, size_t h) : width(static_cast<int>(w)), height(static_cast<int>(h)) {}

IntSize::IntSize(int w, int h) : width(w), height(h) {}

IntSize::IntSize(float w, float h) : width(static_cast<int>(w)), height(static_cast<int>(h)) {}

IntSize::IntSize(const IntSize &other) : width(other.width), height(other.height) {}

IntSize::IntSize(const FloatSize &floatSize) : IntSize(floatSize.width, floatSize.height) {}

IntSize &IntSize::operator=(const IntSize &other) {
    setSize(other.width, other.height);
    return *this;
}

IntSize &IntSize::operator=(const FloatSize &point) {
    setSize(static_cast<int>(point.width), static_cast<int>(point.height));
    return *this;
}

IntSize IntSize::operator+(const IntSize &right) const {
    return IntSize(this->width + right.width, this->height + right.height);
}

IntSize IntSize::operator-(const IntSize &right) const {
    return IntSize(this->width - right.width, this->height - right.height);
}

IntSize IntSize::operator*(int a) const {
    return IntSize(this->width * a, this->height * a);
}

IntSize IntSize::operator/(int a) const {
    return IntSize(this->width / a, this->height / a);
}

void IntSize::setSize(int w, int h) {
    this->width = w;
    this->height = h;
}

bool IntSize::equals(const IntSize &target) const {
    return this->width == target.width && this->height == target.height;
}

void IntSize::scale(float scale) {
    width = static_cast<int>(width * scale);
    height = static_cast<int>(height * scale);
}

const IntSize IntSize::ZERO = IntSize(0, 0);

}