//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CanvasStyle.h"


namespace AtomGraphics {

CanvasStyle::CanvasStyle() : m_isColorStyle(true) {

}


CanvasStyle::CanvasStyle(const Color4F &color)
        : m_isColorStyle(true), m_color(color) {

}

CanvasStyle::CanvasStyle(std::shared_ptr<Gradient> canvasGradient)
        : m_isGradientStyle(true), m_gradient(canvasGradient) {
}

CanvasStyle::CanvasStyle(std::shared_ptr<Pattern> canvasPattern)
        : m_isPatternStyle(true), m_pattern(canvasPattern) {
}

CanvasStyle::CanvasStyle(const CanvasStyle &style) {
    m_isPatternStyle = style.m_isPatternStyle;
    m_isGradientStyle = style.m_isGradientStyle;
    m_isColorStyle = style.m_isColorStyle;
    m_color = style.m_color;
    m_pattern = style.m_pattern;
    m_gradient = style.m_gradient;
}

bool CanvasStyle::isColorStyle() const {
    return m_isColorStyle;
}

bool CanvasStyle::isGradientStyle() const {
    return m_isGradientStyle;
}

bool CanvasStyle::isPatternStyle() const {
    return m_isPatternStyle;
}

const Color4F &CanvasStyle::getColor() const {
    return m_color;
}

std::shared_ptr<Pattern> CanvasStyle::getCanvasPattern() const {
    return m_pattern;
}

std::shared_ptr<Gradient> CanvasStyle::getCanvasGradient() const {
    return m_gradient;
}

#if PLATFORM(ANDROID)

void CanvasStyle::applyToPaint(SkPaint &paint) {
    if (m_isColorStyle) {
        paint.setShader(nullptr);
    } else if (m_isPatternStyle) {
        m_pattern->applyToPaint(paint);
    } else if (m_isGradientStyle) {
        m_gradient->applyToPaint(paint);
    }
}

#endif

CanvasStyle &CanvasStyle::operator=(const CanvasStyle &other) {
    m_isColorStyle = other.m_isColorStyle;
    m_isGradientStyle = other.m_isGradientStyle;
    m_isPatternStyle = other.m_isPatternStyle;

    m_color = other.m_color;
    m_pattern = other.m_pattern;
    m_gradient = other.m_gradient;
    return *this;
}

bool CanvasStyle::operator==(const CanvasStyle &rhs) const {
    return (m_isColorStyle && rhs.m_isColorStyle && m_color == rhs.m_color)
            || (m_isGradientStyle && rhs.m_isGradientStyle && m_gradient == rhs.m_gradient) // FIXME: pattern comparison
            || (m_isPatternStyle && rhs.m_isPatternStyle && m_pattern == rhs.m_pattern);// FIXME: gradient comparison
}

bool CanvasStyle::operator!=(const CanvasStyle &rhs) const {
    return !(rhs == *this);
}

}