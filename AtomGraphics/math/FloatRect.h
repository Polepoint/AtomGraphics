//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FLOATRECT_H
#define ATOMGRAPHICS_FLOATRECT_H

#include "FloatSize.h"
#include "platform/AtomPlatformConfig.h"
#include "FloatPoint.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CGGeometry.h>

#endif

#if PLATFORM(ANDROID)

#include <skia/core/SkRect.h>

#endif

namespace AtomGraphics {

class IntRect;

class FloatRect {

public:

    FloatPoint origin;
    FloatSize size;

    FloatRect();

    FloatRect(float x, float y, float width, float height);

    FloatRect(const FloatPoint &pos, const FloatSize &dimension);

    FloatRect(const FloatRect &other);

    FloatRect(const IntRect &other);

    FloatRect &operator=(const FloatRect &other);

#if PLATFORM(IOS)

    FloatRect(const CGRect &cgRect);
    
    operator CGRect() const;

#elif PLATFORM(ANDROID)

    FloatRect(const SkRect &skRect);

    operator SkRect() const;

#endif

    void setRect(float x, float y, float width, float height);

    float x() const;

    float y() const;

    float width() const;

    void setWidth(float width);

    float height() const;

    void setHeight(float height);

    float getMinX() const;

    float getMidX() const;

    float getMaxX() const;

    float getMinY() const;

    float getMidY() const;

    float getMaxY() const;

    bool equals(const FloatRect &rect) const;

    bool containsPoint(const FloatPoint &point) const;

    bool containsRect(const FloatRect &rect) const;

    bool intersectsRect(const FloatRect &rect) const;

    bool intersectsCircle(const FloatPoint &center, float radius) const;

    void unite(const FloatRect &rect);

    void merge(const FloatRect &rect);

    void move(const FloatSize &size) { origin.move(size.width, size.height); }

    void move(float dx, float dy) { origin.move(dx, dy); }

    void inflateX(float dx) {
        origin.x = origin.x - dx;
        size.width = size.width + dx + dx;
    }

    void inflateY(float dy) {
        origin.y = origin.y - dy;
        size.height = size.height + dy + dy;
    }

    void inflate(float d) {
        inflateX(d);
        inflateY(d);
    }

    void scale(float s);

    void scale(float sx, float sy);

    void intersect(const FloatRect &other);

    bool isEmpty() const;

    static const FloatRect ZERO;
};

IntRect roundedIntRect(const FloatRect &);

FloatRect mapRect(const FloatRect &r, const FloatRect &srcRect, const FloatRect &destRect);

inline bool operator!=(const FloatRect &a, const FloatRect &b) {
    return a.origin != b.origin || a.size != b.size;
}

}

#endif //ATOMGRAPHICS_FLOATRECT_H
