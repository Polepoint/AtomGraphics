//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <cmath>
#include "AffineTransform.h"

namespace AtomGraphics {

AffineTransform::AffineTransform() {

}

AffineTransform::AffineTransform(double a, double b, double c, double d, double tx, double ty)
        : m_a(a), m_b(b), m_c(c), m_d(d), m_tx(tx), m_ty(ty) {
    
}

AffineTransform &AffineTransform::translate(double tx, double ty) {
    if (isIdentityOrTranslation()) {
        m_tx += tx;
        m_ty += ty;
        return *this;
    }
    
    m_tx += tx * m_a + ty * m_c;
    m_ty += tx * m_b + ty * m_d;
    return *this;
}

AffineTransform &AffineTransform::scale(double sx, double sy) {
    m_a *= sx;
    m_b *= sx;
    m_c *= sy;
    m_d *= sy;
    return *this;
}

AffineTransform &AffineTransform::rotate(double a) {
    // angle is in degree. Switch to radian
    a *= M_PI / 180.0;
    double cosAngle = cos(a);
    double sinAngle = sin(a);
    AffineTransform rot(cosAngle, sinAngle, -sinAngle, cosAngle, 0, 0);
    
    *this *= rot;
    return *this;
}

FloatPoint AffineTransform::mapPoint(double x, double y) const {
    double x2, y2;
    x2 = (m_a * x + m_c * y + m_tx);
    y2 = (m_b * x + m_d * y + m_ty);
    
    return FloatPoint(static_cast<float>(x2), static_cast<float>(y2));
}

IntSize AffineTransform::mapSize(const IntSize &size) const {
    double width2 = size.width * xScale();
    double height2 = size.height * yScale();
    
    return IntSize(roundf(static_cast<float>(width2)), roundf(static_cast<float>(height2)));
}

FloatSize AffineTransform::mapSize(const FloatSize &size) const {
    double width2 = size.width * xScale();
    double height2 = size.height * yScale();
    
    return FloatSize(roundf(static_cast<float>(width2)), roundf(static_cast<float>(height2)));
}

FloatQuad AffineTransform::mapQuad(const FloatQuad &q) const {
    if (isIdentityOrTranslation()) {
        FloatQuad mappedQuad(q);
        mappedQuad.move(static_cast<float>(m_tx), static_cast<float>(m_tx));
        return mappedQuad;
    }
    
    FloatQuad result;
    result.setP1(mapPoint(q.p1()));
    result.setP2(mapPoint(q.p2()));
    result.setP3(mapPoint(q.p3()));
    result.setP4(mapPoint(q.p4()));
    return result;
}

double AffineTransform::det() const {
    return m_a * m_d - m_b * m_c;
}

bool AffineTransform::isIdentityOrTranslation() const {
    return m_a == 1 && m_b == 0 && m_c == 0 && m_d == 1;
}

bool AffineTransform::isIdentityOrTranslationOrFlipped() const {
    return m_a == 1 && m_b == 0 && m_c == 0 && (m_d == 1 || m_d == -1);
}

double AffineTransform::xScale() const {
    return sqrt(m_a * m_a + m_b * m_b);
}

double AffineTransform::yScale() const {
    return sqrt(m_c * m_c + m_d * m_d);
}


bool AffineTransform::isInvertible() const {
    float determinant = det();
    return !(!std::isfinite(determinant) || determinant == 0);
    
}

bool AffineTransform::inverse(AffineTransform &inverseValue) const {
    float determinant = det();
    if (!std::isfinite(determinant) || determinant == 0)
        return false;
    
    AffineTransform result;
    if (isIdentityOrTranslation()) {
        result.m_tx = -m_tx;
        result.m_ty = -m_ty;
        inverseValue = result;
        return true;
    }
    
    result.m_a = m_d / determinant;
    result.m_b = -m_b / determinant;
    result.m_c = -m_c / determinant;
    result.m_d = m_a / determinant;
    result.m_tx = (m_c * m_ty - m_d * m_tx) / determinant;
    result.m_ty = (m_b * m_tx - m_a * m_ty) / determinant;
    
    inverseValue = result;
    return true;
}

bool AffineTransform::isIdentity() const {
    return (m_a == 1 && m_b == 0 && m_c == 0 && m_d == 1 && m_tx == 0 && m_ty == 0);
}

FloatRect AffineTransform::mapRect(const FloatRect &rect) const {
    FloatPoint tl = rect.origin;
    FloatPoint tr = FloatPoint(rect.origin.x + rect.size.width, rect.origin.y);
    FloatPoint bl = FloatPoint(rect.origin.x, rect.origin.y + rect.size.height);
    FloatPoint br = FloatPoint(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
    if (isIdentity()) {
        Vec2 v = Vec2(static_cast<float>(m_tx), static_cast<float>(m_ty));
        tl += v;
        tr += v;
        bl += v;
        return FloatRect(tl.x, tl.y, tr.x - tl.x, bl.y - tl.y);
    } else {
        FloatPoint tl1 = mapPoint(tl);
        FloatPoint tr1 = mapPoint(tr);
        FloatPoint bl1 = mapPoint(bl);
        return FloatRect(tl1.x, tl1.y, tr1.x - tl1.x, bl1.y - tl1.y);
    }
}

FloatPoint AffineTransform::mapPoint(const FloatPoint &point) const {
    return FloatPoint(static_cast<float>(m_a * point.x + m_c * point.y + m_tx),
            static_cast<float>(m_b * point.x + m_d * point.y + m_ty));
}

}
