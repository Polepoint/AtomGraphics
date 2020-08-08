//
// Created by neo on 2018/3/30.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMPATH_H
#define ATOMGRAPHICS_ATOMPATH_H


#include "math/Vec2.h"
#include "AffineTransform.h"
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CGPath.h>

#elif PLATFORM(ANDROID)

#include <skia/core/SkPath.h>

#endif

namespace AtomGraphics {

class Path {

public:

    Path();

    Path(const Path &other);

    ~Path();

    void release();

    void addLineTo(float x, float y);

    void moveTo(float x, float y);

    void addQuadCurveTo(float cpx, float cpy, float x, float y);

    void addBezierCurveTo(float cp1x, float cp1y,
                          float cp2x, float cp2y,
                          float x, float y);

    FloatPoint currentPoint();

    void closePath();

    FloatRect boundingRect() const;

    FloatRect fastBoundingRect() const;

    bool isEmpty() const;

    void addArc(float x, float y, float radius, float startAngle, float endAngle,
                bool counterclockwise);

    void addArcToPoint(float x1, float y1, float x2, float y2, float radius);

    void addRect(const FloatRect &);

    bool containsPoint(float x, float y);

    void transform(const AffineTransform &ctm);

    void translate(const FloatSize &);

    void clear();

    bool isNull() const;

    bool hasCurrentPoint() const;

    FloatPoint currentPoint() const;

#if PLATFORM(IOS)

    CGPathRef getCGPath() const;

#endif

#if PLATFORM(ANDROID)

    const SkPath &getSkPath() const {
        return m_skPath;
    }

    void setFillType(SkPath::FillType ft) {
        m_skPath.setFillType(ft);
    }

#endif

private:

#if PLATFORM(IOS)
    mutable CGMutablePathRef m_path{nullptr};

    CGMutablePathRef getMutableCGPath() const;

#endif

#if PLATFORM(ANDROID)
    SkPath m_skPath;
#endif

};

}


#endif //ATOMGRAPHICS_ATOMPATH_H
