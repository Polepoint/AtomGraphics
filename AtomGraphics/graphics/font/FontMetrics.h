//
// Created by neo on 2018/8/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FONTMETRICS_H
#define ATOMGRAPHICS_FONTMETRICS_H

#include <math.h>
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkPaint.h>

#endif

namespace AtomGraphics {

class FontMetrics {

public:

    FontMetrics() = default;

#if PLATFORM(ANDROID)

    FontMetrics(const SkPaint::FontMetrics &metrics) {
        float ascent = SkScalarRoundToScalar(-metrics.fAscent);
        float descent = SkScalarRoundToScalar(metrics.fDescent);
        float line_gap = SkScalarToFloat(metrics.fLeading);
        m_ascent = ascent;
        m_descent = descent;
        m_lineGap = line_gap;
        m_lineSpacing = lroundf(ascent) + lroundf(descent) + lroundf(line_gap);
        m_xHeight = metrics.fXHeight;
        m_capHeight = metrics.fCapHeight;
    }

#endif

    bool initialized() const {
        return m_initialized;
    }

    float ascent() const {
        return m_ascent;
    }

    void setAscent(float ascent) {
        m_ascent = ascent;
    }

    float descent() const {
        return m_descent;
    }

    void setDescent(float descent) {
        m_descent = descent;
    }

    float lineGap() const {
        return m_lineGap;
    }

    void setLineGap(float lineGap) {
        m_lineGap = lineGap;
    }

    float lineSpacing() const {
        return m_lineSpacing;
    }

    void setLineSpacing(float lineSpacing) {
        m_lineSpacing = lineSpacing;
    }

    float height() const {
        return m_ascent + m_descent;
    }

    float xHeight() const {
        return m_xHeight;
    }

    void setXHeight(float xHeight) {
        m_xHeight = xHeight;
    }

    float capHeight() const {
        return m_capHeight;
    }

    void setCapHeight(float capHeight) {
        m_capHeight = capHeight;
    }

private:

    friend class Font;

    void reset() {
        m_initialized = false;
        m_ascent = 0;
        m_descent = 0;
        m_lineGap = 0;
        m_lineSpacing = 0;
        m_xHeight = 0;
        m_capHeight = 0;
    }

    bool m_initialized{false};
    float m_ascent{0};
    float m_descent{0};
    float m_lineGap{0};
    float m_lineSpacing{0};
    float m_xHeight{0};
    float m_capHeight{0};
};
}


#endif //ATOMGRAPHICS_FONTMETRICS_H
