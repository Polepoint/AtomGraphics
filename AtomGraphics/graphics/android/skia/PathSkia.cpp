//
// Created by neo on 2018/9/5.
//

#include "graphics/Path.h"

#define TWO_PI static_cast<float>(M_PI)*2

namespace AtomGraphics {


Path::Path() {}

Path::Path(const Path &other) {
    m_skPath = other.m_skPath;
}

Path::~Path() {}

void Path::release() {}

void Path::addLineTo(float x, float y) {
    m_skPath.lineTo(SkFloatToScalar(x), SkFloatToScalar(y));
}

void Path::moveTo(float x, float y) {
    m_skPath.moveTo(SkFloatToScalar(x), SkFloatToScalar(y));
}

void Path::addQuadCurveTo(float cpx, float cpy, float x, float y) {
    m_skPath.quadTo(SkFloatToScalar(cpx), SkFloatToScalar(cpy),
                    SkFloatToScalar(x), SkFloatToScalar(y));
}

void Path::addBezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    m_skPath.cubicTo(SkFloatToScalar(cp1x), SkFloatToScalar(cp1y),
                     SkFloatToScalar(cp2x), SkFloatToScalar(cp2y),
                     SkFloatToScalar(x), SkFloatToScalar(y));
}

FloatPoint Path::currentPoint() {
    SkPoint currentPoint;
    m_skPath.getLastPt(&currentPoint);
    return FloatPoint(currentPoint.y(), currentPoint.y());
}

void Path::closePath() {
    m_skPath.close();
}

FloatRect Path::boundingRect() const {
    SkRect rect = m_skPath.computeTightBounds();
    return FloatRect(rect.x(), rect.y(), rect.width(), rect.height());
}

bool Path::isEmpty() const {
    return m_skPath.isEmpty();
}

inline float adjustEndAngle(float start_angle, float end_angle, bool anticlockwise) {
    float new_end_angle = end_angle;
    if (!anticlockwise && end_angle - start_angle >= TWO_PI) {
        new_end_angle = start_angle + TWO_PI;
    } else if (anticlockwise && start_angle - end_angle >= TWO_PI) {
        new_end_angle = start_angle - TWO_PI;
    } else if (!anticlockwise && start_angle > end_angle) {
        new_end_angle = start_angle + (TWO_PI - fmodf(start_angle - end_angle, TWO_PI));
    } else if (anticlockwise && start_angle < end_angle) {
        new_end_angle = start_angle - (TWO_PI - fmodf(end_angle - start_angle, TWO_PI));
    }

    return new_end_angle;
}

void Path::addArc(float x, float y, float radius, float startAngle, float endAngle, bool counterclockwise) {
    endAngle = adjustEndAngle(startAngle, endAngle, counterclockwise);
    SkScalar cx = SkFloatToScalar(x);
    SkScalar cy = SkFloatToScalar(y);
    SkScalar radius_scalar = SkFloatToScalar(radius);

    SkRect oval;
    oval.set(cx - radius_scalar, cy - radius_scalar, cx + radius_scalar, cy + radius_scalar);

    float sweep = endAngle - startAngle;
    SkScalar start_degrees = SkFloatToScalar(startAngle * 180 / M_PI);
    SkScalar sweep_degrees = SkFloatToScalar(sweep * 180 / M_PI);
    SkScalar s360 = SkIntToScalar(360);

    // We can't use SkPath::addOval(), because addOval() makes a new sub-path.
    // addOval() calls moveTo() and close() internally.

    // Use s180, not s360, because SkPath::arcTo(oval, angle, s360, false) draws
    // nothing.
    SkScalar s180 = SkIntToScalar(180);
    if (SkScalarNearlyEqual(sweep_degrees, s360)) {
        // SkPath::arcTo can't handle the sweepAngle that is equal to or greater
        // than 2Pi.
        m_skPath.arcTo(oval, start_degrees, s180, false);
        m_skPath.arcTo(oval, start_degrees + s180, s180, false);
        return;
    }
    if (SkScalarNearlyEqual(sweep_degrees, -s360)) {
        m_skPath.arcTo(oval, start_degrees, -s180, false);
        m_skPath.arcTo(oval, start_degrees - s180, -s180, false);
        return;
    }

    m_skPath.arcTo(oval, start_degrees, sweep_degrees, false);
}

void Path::addArcToPoint(float x1, float y1, float x2, float y2, float radius) {
    m_skPath.arcTo(SkFloatToScalar(x1), SkFloatToScalar(y1),
                   SkFloatToScalar(x2), SkFloatToScalar(y2),
                   SkFloatToScalar(radius));
}

void Path::addRect(const FloatRect &rect) {
    m_skPath.addRect(SkRect::MakeXYWH(
            SkFloatToScalar(rect.origin.x),
            SkFloatToScalar(rect.origin.y),
            SkFloatToScalar(rect.size.width),
            SkFloatToScalar(rect.size.height)));
}

bool Path::containsPoint(float x, float y) {
    return m_skPath.contains(SkFloatToScalar(x), SkFloatToScalar(y));
}

void Path::transform(const AffineTransform &ctm) {
    m_skPath.transform(ctm);
}

FloatRect Path::fastBoundingRect() const {
    return m_skPath.computeTightBounds();
}

void Path::translate(const FloatSize &size) {
    transform(AffineTransform(1, 0, 0, 1, size.width, size.height));
}

void Path::clear() {
    m_skPath.reset();
}

bool Path::isNull() const {
    return false;
}

bool Path::hasCurrentPoint() const {
    return !m_skPath.isEmpty();
}

FloatPoint Path::currentPoint() const {
    return m_skPath.getPoint(std::max(0, m_skPath.countPoints() - 1));
}


}