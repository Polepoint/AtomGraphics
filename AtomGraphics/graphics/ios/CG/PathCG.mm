//
// Created by neo on 2018/11/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "Path.h"

namespace AtomGraphics {

Path::Path() {}

Path::Path(const Path &other) {
    m_path = other.m_path;
    CGPathRetain(m_path);
}

Path::~Path() {
    if (m_path) {
        CGPathRelease(m_path);
    }
}

void Path::release() {

}

void Path::addLineTo(float x, float y) {
    CGPathAddLineToPoint(getMutableCGPath(), nullptr, x, y);
}

void Path::moveTo(float x, float y) {
    CGPathMoveToPoint(getMutableCGPath(), nullptr, x, y);
}

void Path::addQuadCurveTo(float cpx, float cpy, float x, float y) {
    CGPathAddQuadCurveToPoint(getMutableCGPath(), nullptr, cpx, cpy, x, y);
}

void Path::addBezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    CGPathAddCurveToPoint(getMutableCGPath(), nullptr, cp1x, cp1y, cp2x, cp2y, x, y);
}

FloatPoint Path::currentPoint() {
    CGPoint currentPoint = CGPathGetCurrentPoint(getCGPath());
    return FloatPoint(currentPoint);
}

void Path::closePath() {
    if (isNull())
        return;

    CGPathCloseSubpath(getMutableCGPath());
}

FloatRect Path::boundingRect() const {
    if (isNull())
        return CGRectZero;

    CGRect bound = CGPathGetPathBoundingBox(m_path);
    return CGRectIsNull(bound) ? CGRectZero : bound;
}

FloatRect Path::fastBoundingRect() const {
    if (isNull())
        return CGRectZero;
    CGRect bound = CGPathGetBoundingBox(m_path);
    return CGRectIsNull(bound) ? CGRectZero : bound;
}

bool Path::isEmpty() const {
    return isNull() || CGPathIsEmpty(m_path);
}

void Path::addArc(float x, float y, float radius, float startAngle, float endAngle, bool counterclockwise) {
    if (std::isfinite(startAngle) && std::isfinite(endAngle))
        CGPathAddArc(getMutableCGPath(), nullptr, x, y, radius, startAngle, endAngle, counterclockwise);
}

void Path::addArcToPoint(float x1, float y1, float x2, float y2, float radius) {
    CGPathAddArcToPoint(getMutableCGPath(), nullptr, x1, y1, x2, y2, radius);
}

void Path::addRect(const FloatRect &r) {
    CGPathAddRect(getMutableCGPath(), 0, r);
}

bool Path::containsPoint(float x, float y) {
    return CGPathContainsPoint(getCGPath(), 0, CGPointMake(x, y), false);
}

void Path::transform(const AffineTransform &ctm) {
    if (ctm.isIdentity() || isEmpty())
        return;

    CGAffineTransform transformCG = ctm;
    CGMutablePathRef path = CGPathCreateMutableCopyByTransformingPath(getCGPath(), &transformCG);
    CGPathRelease(m_path);
    m_path = path;
}

void Path::translate(const FloatSize &size) {
    transform(AffineTransform(1, 0, 0, 1, size.width, size.height));
}

void Path::clear() {
    if (isNull())
        return;

    CGPathRelease(m_path);
    m_path = CGPathCreateMutable();

}

CGPathRef Path::getCGPath() const {
    if (!m_path) {
        return getMutableCGPath();
    }

    return m_path;
}

CGMutablePathRef Path::getMutableCGPath() const {
    if (!m_path) {
        m_path = CGPathCreateMutable();
    }

    return m_path;
}

bool Path::isNull() const {
    return !m_path;
}

bool Path::hasCurrentPoint() const {
    return !isEmpty();
}

FloatPoint Path::currentPoint() const {
    if (isNull())
        return FloatPoint();
    return CGPathGetCurrentPoint(m_path);
}

}