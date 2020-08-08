//
// Created by neo on 2018/11/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FLOATPOINT_H
#define ATOMGRAPHICS_FLOATPOINT_H

#include "Vec2.h"
#include "platform/AtomPlatformConfig.h"
#include "FloatSize.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CoreGraphics.h>

#endif

#if PLATFORM(ANDROID)

#include <skia/core/SkPoint.h>

#endif

namespace AtomGraphics {

class IntPoint;

class FloatPoint : public Vec2 {

public:

#if PLATFORM(IOS)

    FloatPoint(const CGPoint &);
    
    operator CGPoint() const;

#endif

#if PLATFORM(ANDROID)

    FloatPoint(const SkPoint &);

    operator SkPoint() const;

#endif

    FloatPoint(const IntPoint &intPoint);

    FloatPoint(const FloatPoint &other);

    FloatPoint();

    FloatPoint(float xx, float yy);

    FloatPoint(const float *array);

    FloatPoint(const Vec2 &p1, const Vec2 &p2);

    FloatPoint(const Vec2 &copy);

    ~FloatPoint() override;

    void move(float dx, float dy);

    void scale(float sx, float sy) {
        x *= sx;
        y *= sy;
    }
};

inline FloatPoint operator+(const FloatPoint &a, const FloatSize &b) {
    return FloatPoint(a.x + b.width, a.y + b.height);
}

inline FloatPoint operator+(const FloatPoint &a, const FloatPoint &b) {
    return FloatPoint(a.x + b.x, a.y + b.y);
}

inline FloatSize operator-(const FloatPoint &a, const FloatPoint &b) {
    return FloatSize(a.x - b.x, a.y - b.y);
}

inline FloatPoint operator-(const FloatPoint &a, const FloatSize &b) {
    return FloatPoint(a.x - b.width, a.y - b.height);
}

inline FloatPoint operator-(const FloatPoint &a) {
    return FloatPoint(-a.x, -a.y);
}

IntPoint roundedIntPoint(const FloatPoint &);

}


#endif //ATOMGRAPHICS_FLOATPOINT_H
