//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_AFFINETRANSFORM_H
#define ATOMGRAPHICS_AFFINETRANSFORM_H

#include "math/AtomGeometry.h"
#include "math/Vec2.h"
#include "math/FloatQuad.h"
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CGAffineTransform.h>

#endif

#if PLATFORM(ANDROID)

#include <skia/core/SkMatrix.h>

#endif

namespace AtomGraphics {

class AffineTransform {


public:

    AffineTransform();

    AffineTransform(double a, double b, double c, double d, double tx, double ty);

#if PLATFORM(IOS)

    AffineTransform(const CGAffineTransform &);

#elif PLATFORM(ANDROID)

    AffineTransform(const SkMatrix &);

#endif

    AffineTransform &translate(double tx, double ty);

    AffineTransform &scale(double sx, double sy);

    AffineTransform &rotate(double a);

    FloatPoint mapPoint(double x, double y) const;

    IntSize mapSize(const IntSize &) const;

    FloatSize mapSize(const FloatSize &) const;

    FloatQuad mapQuad(const FloatQuad &) const;

    bool isInvertible() const;

    bool inverse(AffineTransform &inverseValue) const;

    bool isIdentity() const;

    FloatRect mapRect(const FloatRect &) const;

    FloatPoint mapPoint(const FloatPoint &) const;

    bool isIdentityOrTranslation() const;

    bool isIdentityOrTranslationOrFlipped() const;

    double xScale() const;

    double yScale() const;

    inline bool operator==(const AffineTransform &other) const {
        return m_a == other.m_a
                && m_b == other.m_b
                && m_c == other.m_c
                && m_d == other.m_d
                && m_tx == other.m_tx
                && m_ty == other.m_ty;
    }

    inline bool operator!=(const AffineTransform &other) const {
        return !(*this == other);
    }

    inline AffineTransform &operator*=(const AffineTransform &rhs) {
        AffineTransform result = *this * rhs;
        *this = result;
        return *this;
    }

    inline AffineTransform operator*(const AffineTransform &other) {
        AffineTransform trans;
        trans.m_a = other.m_a * m_a + other.m_b * m_c;
        trans.m_b = other.m_a * m_b + other.m_b * m_d;
        trans.m_c = other.m_c * m_a + other.m_d * m_c;
        trans.m_d = other.m_c * m_b + other.m_d * m_d;
        trans.m_tx = other.m_tx * m_a + other.m_ty * m_c + m_tx;
        trans.m_ty = other.m_tx * m_b + other.m_ty * m_d + m_ty;

        return trans;
    }

    AffineTransform operator*(const AffineTransform &t) const {
        AffineTransform result = *this;
        result *= t;
        return result;
    }

    inline AffineTransform &operator=(const AffineTransform &other) {
        this->m_a = other.m_a;
        this->m_b = other.m_b;
        this->m_c = other.m_c;
        this->m_d = other.m_d;
        this->m_tx = other.m_tx;
        this->m_ty = other.m_ty;

        return *this;
    }

#if PLATFORM(IOS)

    operator CGAffineTransform() const;

#endif

#if PLATFORM(ANDROID)

    operator SkMatrix() const;

#endif

private:
    double m_a{1};
    double m_b{0};
    double m_c{0};
    double m_d{1};
    double m_tx{0};
    double m_ty{0};

    double det() const;

};

}


#endif //ATOMGRAPHICS_AFFINETRANSFORM_H
