//
// Created by neo on 2019/2/12.
//

#include <skia/core/SkColorFilter.h>
#include <skia/core/SkShader.h>
#include <skia/effects/SkGradientShader.h>
#include "graphics/Gradient.h"
#include "graphics/android/skia/SkiaUtils.h"

namespace AtomGraphics {

void Gradient::platformDestroy() {
    m_cachedShader.reset();
}

void Gradient::applyToPaint(SkPaint &paint) {
    if (!m_cachedShader || paint.getColorFilter() != m_colorFilter.get()) {
        m_colorFilter = paint.refColorFilter();
        paint.setColorFilter(nullptr);
        m_cachedShader = platformGradient();
    }

    paint.setShader(m_cachedShader);

    // Legacy behavior: gradients are always dithered.
    paint.setDither(true);
}

sk_sp<SkShader> Gradient::platformGradient() {
    sortStopsIfNecessary();

    std::vector<SkColor> colors;
    std::vector<SkScalar> pos;

    fillSkiaStops(colors, pos);
    sk_sp<SkShader> shader = CreateShader(colors, pos, SkShader::kClamp_TileMode);

    return shader;
}

void Gradient::fillSkiaStops(std::vector<SkColor> &colors, std::vector<SkScalar> &pos) const {
    if (m_stops.empty()) {
        // A gradient with no stops must be transparent black.
        pos.push_back(WebCoreFloatToSkScalar(0));
        colors.push_back(SK_ColorTRANSPARENT);
    } else if (m_stops.front().offset > 0) {
        // Copy the first stop to 0.0. The first stop position may have a slight
        // rounding error, but we don't care in this float comparison, since
        // 0.0 comes through cleanly and people aren't likely to want a gradient
        // with a stop at (0 + epsilon).
        pos.push_back(WebCoreFloatToSkScalar(0));
        if (m_colorFilter) {
            colors.push_back(m_colorFilter->filterColor(ColorToSkColor(m_stops.front().color)));
        } else {
            colors.push_back(ColorToSkColor(m_stops.front().color));
        }
    }

    for (const auto &stop : m_stops) {
        pos.push_back(WebCoreFloatToSkScalar(stop.offset));
        if (m_colorFilter)
            colors.push_back(m_colorFilter->filterColor(ColorToSkColor(stop.color)));
        else
            colors.push_back(ColorToSkColor(stop.color));
    }

    // Copy the last stop to 1.0 if needed. See comment above about this float
    // comparison.
    if (pos.back() < 1) {
        pos.push_back(WebCoreFloatToSkScalar(1));
        colors.push_back(colors.back());
    }
}

sk_sp<SkShader> Gradient::CreateShader(const std::vector<SkColor> &colors,
                                             const std::vector<SkScalar> &pos,
                                             SkShader::TileMode tile_mode) {

    if (m_isLinear) {
        SkPoint pts[2] = {m_point0, m_point1};
        return SkGradientShader::MakeLinear(
                pts, colors.data(), pos.data(), static_cast<int>(colors.size()),
                tile_mode, 0, &SkMatrix::I());
    } else {
        // The radii we give to Skia must be positive. If we're given a
        // negative radius, ask for zero instead.
        const SkScalar radius0 = std::max(WebCoreFloatToSkScalar(m_startRadius), 0.0f);
        const SkScalar radius1 = std::max(WebCoreFloatToSkScalar(m_endRadius), 0.0f);
        return SkGradientShader::MakeTwoPointConical(
                m_point0, radius0, m_point1, radius1,
                colors.data(), pos.data(),
                static_cast<int>(colors.size()),
                tile_mode, 0, &SkMatrix::I());
    }
}

}