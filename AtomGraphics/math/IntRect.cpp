//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "IntRect.h"
#include "FloatRect.h"
#include <algorithm>
#include <utility>

namespace AtomGraphics {

IntRect::IntRect(void) {
    setRect(0, 0, 0, 0);
}

IntRect::IntRect(const IntRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

IntRect::IntRect(int x, int y, int width, int height) {
    setRect(x, y, width, height);
}

IntRect::IntRect(const IntPoint &point, const IntSize &intSize) {
    origin = point;
    size = intSize;
}

IntRect::IntRect(const FloatRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

int IntRect::x() const {
    return origin.x;
}

int IntRect::y() const {
    return origin.y;
}

int IntRect::width() const {
    return size.width;
}

int IntRect::height() const {
    return size.height;
}

IntRect &IntRect::operator=(const IntRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

IntRect &IntRect::operator=(const FloatRect &other) {
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

void IntRect::setRect(int x, int y, int width, int height) {
    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

void IntRect::setRect(float x, float y, float width, float height) {
    setRect(static_cast<int>(x), static_cast<int>(y),
            static_cast<int>(width), static_cast<int>(height));
}

bool IntRect::equals(const IntRect &rect) const {
    return (origin.x == rect.origin.x &&
            origin.y == rect.origin.y &&
            size.equals(rect.size));
}

int IntRect::getMaxX() const {
    return origin.x + size.width;
}

int IntRect::getMidX() const {
    return origin.x + size.width / 2;
}

int IntRect::getMinX() const {
    return origin.x;
}

int IntRect::getMaxY() const {
    return origin.y + size.height;
}

int IntRect::getMidY() const {
    return origin.y + size.height / 2;
}

int IntRect::getMinY() const {
    return origin.y;
}

bool IntRect::containsPoint(const IntPoint &point) const {
    bool bRet = false;

    if (point.x >= getMinX() && point.x <= getMaxX()
            && point.y >= getMinY() && point.y <= getMaxY()) {
        bRet = true;
    }

    return bRet;
}

bool IntRect::containsRect(const IntRect &rect) const {

    return containsPoint(rect.origin) &&
            containsPoint({rect.origin.x + rect.size.width,
                           rect.origin.y + rect.size.height});

}

bool IntRect::intersectsRect(const IntRect &rect) const {
    return !(getMaxX() < rect.getMinX() ||
            rect.getMaxX() < getMinX() ||
            getMaxY() < rect.getMinY() ||
            rect.getMaxY() < getMinY());
}

void IntRect::unite(const IntRect &rect) {
    int thisLeftX = origin.x;
    int thisRightX = origin.x + size.width;
    int thisTopY = origin.y + size.height;
    int thisBottomY = origin.y;

    if (thisRightX < thisLeftX) {
        std::swap(thisRightX, thisLeftX);   // This rect has negative width
    }

    if (thisTopY < thisBottomY) {
        std::swap(thisTopY, thisBottomY);   // This rect has negative height
    }

    int otherLeftX = rect.origin.x;
    int otherRightX = rect.origin.x + rect.size.width;
    int otherTopY = rect.origin.y + rect.size.height;
    int otherBottomY = rect.origin.y;

    if (otherRightX < otherLeftX) {
        std::swap(otherRightX, otherLeftX);   // Other rect has negative width
    }

    if (otherTopY < otherBottomY) {
        std::swap(otherTopY, otherBottomY);   // Other rect has negative height
    }

    int combinedLeftX = std::min(thisLeftX, otherLeftX);
    int combinedRightX = std::max(thisRightX, otherRightX);
    int combinedTopY = std::max(thisTopY, otherTopY);
    int combinedBottomY = std::min(thisBottomY, otherBottomY);

    origin.x = combinedLeftX;
    origin.y = combinedBottomY;
    size.width = combinedRightX - combinedLeftX;
    size.height = combinedTopY - combinedBottomY;
}

void IntRect::merge(const IntRect &rect) {
    int minX = std::min(getMinX(), rect.getMinX());
    int minY = std::min(getMinY(), rect.getMinY());
    int maxX = std::max(getMaxX(), rect.getMaxX());
    int maxY = std::max(getMaxY(), rect.getMaxY());
    setRect(minX, minY, maxX - minX, maxY - minY);
}

void IntRect::scale(float scale) {
    origin.x *= scale;
    origin.y *= scale;
    size.width *= scale;
    size.height *= scale;
}

void IntRect::intersect(const IntRect &other) {
    int l = std::max(origin.x, other.origin.x);
    int t = std::max(origin.y, other.origin.y);
    int r = std::min(origin.x + size.width, other.origin.x + other.size.width);
    int b = std::min(origin.y + size.height, other.origin.y + other.size.height);

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

bool IntRect::isEmpty() const {
    return size.isEmpty();
}


const IntRect IntRect::ZERO = IntRect(0, 0, 0, 0);


IntRect enclosingIntRect(const FloatRect &rect) {
    int floorX = static_cast<int>(floor(rect.origin.x));
    int floorY = static_cast<int>(floor(rect.origin.y));
    int ceilMaxX = static_cast<int>(ceil(rect.getMaxX()));
    int ceilMaxY = static_cast<int>(ceil(rect.getMaxY()));
    return IntRect(floorX, floorY, ceilMaxX - floorX, ceilMaxY - floorY);
}

}