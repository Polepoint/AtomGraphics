//
// Created by neo on 2018/4/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRADIENT_H
#define ATOMGRADIENT_H

#include <vector>
#include "base/AtomTypes.h"
#include "math/AtomGeometry.h"

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkPaint.h>
#include <skia/core/SkShader.h>
#include <skia/core/SkColorFilter.h>

#endif

#if PLATFORM(IOS)

#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGPath.h>

#endif

namespace AtomGraphics {

class Gradient {

public:

    struct ColorStop {
        float offset{0};
        Color4F color;

        ColorStop() = default;

        ColorStop(float offset, const Color4F &color)
                : offset(offset), color(color) {
        }
    };

    Gradient(float x0, float y0, float x1, float y1) : m_isLinear(true), m_isRadial(false) {
        m_point0.x = x0;
        m_point0.y = y0;

        m_point1.x = x1;
        m_point1.y = y1;
    }

    Gradient(float x0, float y0, float r0, float x1, float y1, float r1)
            : m_isLinear(false), m_isRadial(true) {
        m_point0.x = x0;
        m_point0.y = y0;
        m_startRadius = r0;

        m_point1.x = x1;
        m_point1.y = y1;
        m_endRadius = r1;
    }

    ~Gradient();

    void addColorStop(float, const Color4F &);

    void addColorStop(const ColorStop &);

    int getStopCount() const {
        return static_cast<int>(m_stops.size());
    }

    const ColorStop *getColorStop(int index) const {
        if (index >= 0 && index < m_stops.size()) {
            return &m_stops.at(index);
        }
        return nullptr;
    }

    bool isZeroSize() const;

    bool isLinearGradient() const {
        return m_isLinear;
    }

    bool isRadialGradient() const {
        return m_isRadial;
    }

    void platformDestroy();

#if PLATFORM(IOS)

    void paint(CGContextRef platformContext);

    CGGradientRef platformGradient();

#elif PLATFORM(ANDROID)

    void applyToPaint(SkPaint &paint);

    sk_sp<SkShader> platformGradient();

#endif

private:

    bool m_isLinear{false};
    bool m_isRadial{false};

    mutable bool m_stopsSorted{false};

    FloatPoint m_point0, m_point1;
    float m_startRadius, m_endRadius;
    std::vector<ColorStop> m_stops;

#if PLATFORM(IOS)

    CGGradientRef m_gradient{nullptr};

#elif PLATFORM(ANDROID)

    sk_sp<SkShader> m_cachedShader;
    sk_sp<SkColorFilter> m_colorFilter;

    void fillSkiaStops(std::vector<SkColor> &colors, std::vector<SkScalar> &pos) const;

    sk_sp<SkShader> CreateShader(const std::vector<SkColor> &, const std::vector<SkScalar> &, SkShader::TileMode);


#endif

    void sortStopsIfNecessary();
};

}


#endif //ATOMGRADIENT_H
