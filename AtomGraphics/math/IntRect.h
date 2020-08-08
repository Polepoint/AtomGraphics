//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_INTRECT_H
#define ATOMGRAPHICS_INTRECT_H

#include "IntSize.h"
#include "IntPoint.h"
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkRect.h>

#endif

namespace AtomGraphics {

class FloatRect;

class IntRect {

public:
    IntPoint origin;
    IntSize size;

    IntRect();

    IntRect(int x, int y, int width, int height);

    IntRect(const IntRect &other);

    IntRect(const FloatRect &floatRect);

    IntRect(const IntPoint &point, const IntSize &intSize);

    int x() const;

    int y() const;

    int width() const;

    int height() const;

    IntRect &operator=(const IntRect &other);

    IntRect &operator=(const FloatRect &other);

    void setRect(int x, int y, int width, int height);

    void setRect(float x, float y, float width, float height);

    int getMinX() const;

    int getMidX() const;

    int getMaxX() const;

    int getMinY() const;

    int getMidY() const;

    int getMaxY() const;

    bool equals(const IntRect &rect) const;

    bool containsPoint(const IntPoint &point) const;

    bool containsRect(const IntRect &rect) const;

    bool intersectsRect(const IntRect &rect) const;

    void unite(const IntRect &rect);

    void move(const IntSize &size) { origin += size; }

    void move(int dx, int dy) { origin.move(dx, dy); }

    void merge(const IntRect &rect);

    void scale(float scale);

    void intersect(const IntRect &other);

    bool isEmpty() const;

    static const IntRect ZERO;

    bool operator==(const IntRect &other) const {
        return this->equals(other);
    }

    bool operator!=(const IntRect &other) const {
        return !this->equals(other);
    }

#if PLATFORM(ANDROID)

    IntRect(const SkIRect &rect);

    operator SkIRect() const;

    operator SkRect() const;

#endif

};

IntRect enclosingIntRect(const FloatRect &);

}

#endif //ATOMGRAPHICS_INTRECT_H
