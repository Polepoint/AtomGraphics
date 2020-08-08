//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "FloatRect.h"
#include "IntRect.h"
#include <algorithm>
#include <utility>

namespace AtomGraphics {

FloatRect::FloatRect(void) {
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

FloatRect::FloatRect(float x, float y, float width, float height) {
    setRect(x, y, width, height);
}

FloatRect::FloatRect(const FloatPoint &pos, const FloatSize &dimension) {
    setRect(pos.x, pos.y, dimension.width, dimension.height);
}

FloatRect::FloatRect(const FloatRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

FloatRect::FloatRect(const IntRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

FloatRect &FloatRect::operator=(const FloatRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

void FloatRect::setRect(float x, float y, float width, float height) {
    // CGRect can support width<0 or height<0
    // ATASSERT(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

float FloatRect::x() const {
    return origin.x;
}

float FloatRect::y() const {
    return origin.y;
}

float FloatRect::width() const {
    return size.width;
}

void FloatRect::setWidth(float width) {
    size.width = width;
}

float FloatRect::height() const {
    return size.height;
}

void FloatRect::setHeight(float height) {
    size.height = height;
}

bool FloatRect::equals(const FloatRect &rect) const {
    return (origin.equals(rect.origin) &&
            size.equals(rect.size));
}

float FloatRect::getMaxX() const {
    return origin.x + size.width;
}

float FloatRect::getMidX() const {
    return origin.x + size.width / 2.0f;
}

float FloatRect::getMinX() const {
    return origin.x;
}

float FloatRect::getMaxY() const {
    return origin.y + size.height;
}

float FloatRect::getMidY() const {
    return origin.y + size.height / 2.0f;
}

float FloatRect::getMinY() const {
    return origin.y;
}

bool FloatRect::containsPoint(const FloatPoint &point) const {
    bool bRet = false;

    if (point.x >= getMinX() && point.x <= getMaxX()
            && point.y >= getMinY() && point.y <= getMaxY()) {
        bRet = true;
    }

    return bRet;
}

bool FloatRect::containsRect(const FloatRect &rect) const {

    return containsPoint(rect.origin) &&
            containsPoint(FloatPoint(rect.origin.x + rect.size.width,
                                     rect.origin.y + rect.size.height));

}

bool FloatRect::intersectsRect(const FloatRect &rect) const {
    return !(getMaxX() < rect.getMinX() ||
            rect.getMaxX() < getMinX() ||
            getMaxY() < rect.getMinY() ||
            rect.getMaxY() < getMinY());
}

bool FloatRect::intersectsCircle(const FloatPoint &center, float radius) const {
    Vec2 rectangleCenter((origin.x + size.width / 2),
                         (origin.y + size.height / 2));

    float w = size.width / 2;
    float h = size.height / 2;

    float dx = std::abs(center.x - rectangleCenter.x);
    float dy = std::abs(center.y - rectangleCenter.y);

    if (dx > (radius + w) || dy > (radius + h)) {
        return false;
    }

    Vec2 circleDistance(std::abs(center.x - origin.x - w),
                        std::abs(center.y - origin.y - h));

    if (circleDistance.x <= (w)) {
        return true;
    }

    if (circleDistance.y <= (h)) {
        return true;
    }

    float cornerDistanceSq = powf(circleDistance.x - w, 2) + powf(circleDistance.y - h, 2);

    return (cornerDistanceSq <= (powf(radius, 2)));
}

void FloatRect::unite(const FloatRect &other) {
    if (other.isEmpty())
        return;
    if (isEmpty()) {
        *this = other;
        return;
    }
    float minX = std::min(x(), other.x());
    float minY = std::min(y(), other.y());
    float maxX = std::max(this->getMaxX(), other.getMaxX());
    float maxY = std::max(this->getMaxY(), other.getMaxY());

    origin.x = minX;
    origin.y = minY;
    size.width = maxX - minX;
    size.height = maxY - minY;
}

void FloatRect::merge(const FloatRect &rect) {
    float minX = std::min(getMinX(), rect.getMinX());
    float minY = std::min(getMinY(), rect.getMinY());
    float maxX = std::max(getMaxX(), rect.getMaxX());
    float maxY = std::max(getMaxY(), rect.getMaxY());
    setRect(minX, minY, maxX - minX, maxY - minY);
}

void FloatRect::scale(float s) {
    scale(s, s);
}

void FloatRect::scale(float sx, float sy) {
    origin.x *= sx;
    origin.y *= sy;
    size.width *= sx;
    size.height *= sy;
}

void FloatRect::intersect(const FloatRect &other) {
    float l = std::max(origin.x, other.origin.x);
    float t = std::max(origin.y, other.origin.y);
    float r = std::min(origin.x + size.width, other.origin.x + other.size.width);
    float b = std::min(origin.y + size.height, other.origin.y + other.size.height);

    // Return a clean empty rectangle for non-intersecting cases.
    if (l >= r || t >= b) {
        l = 0;
        t = 0;
        r = 0;
        b = 0;
    }

    origin.x = l;
    origin.y = t;
    size.width = r - l;
    size.height = b - t;
}

bool FloatRect::isEmpty() const {
    return size.isEmpty();
}

const FloatRect FloatRect::ZERO = FloatRect(0, 0, 0, 0);

IntRect roundedIntRect(const FloatRect &rect) {
    return IntRect(roundedIntPoint(rect.origin), roundedIntSize(rect.size));
}

FloatRect mapRect(const FloatRect &r, const FloatRect &srcRect, const FloatRect &destRect) {
    if (!srcRect.width() || !srcRect.height())
        return FloatRect();

    float widthScale = destRect.width() / srcRect.width();
    float heightScale = destRect.height() / srcRect.height();
    return {
            destRect.x() + (r.x() - srcRect.x()) * widthScale,
            destRect.y() + (r.y() - srcRect.y()) * heightScale,
            r.width() * widthScale,
            r.height() * heightScale
    };
}

}