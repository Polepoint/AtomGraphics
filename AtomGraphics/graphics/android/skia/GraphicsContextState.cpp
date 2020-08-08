//
// Created by neo on 2018/9/4.
//

#include <cmath>
#include <skia/effects/SkDashPathEffect.h>
#include <skia/effects/SkLayerDrawLooper.h>
#include <skia/core/SkMaskFilter.h>
#include <skia/core/SkColorFilter.h>
#include "graphics/GraphicsTypes.h"
#include "GraphicsContextState.h"
#include "SkiaUtils.h"

namespace AtomGraphics {


static inline SkFilterQuality FilterQualityForPaint(
        InterpolationQuality quality) {
    // The filter quality "selected" here will primarily be used when painting a
    // primitive using one of the PaintFlags below. For the most part this will
    // not affect things that are part of the Image class hierarchy (which use
    // the unmodified m_interpolationQuality.)
    return quality != InterpolationNone ? kLow_SkFilterQuality
                                        : kNone_SkFilterQuality;
}

inline SkColor ScaleAlpha(SkColor color, float alpha) {
    const auto clamped_alpha = std::max(0.0f, std::min(1.0f, alpha));
    const auto rounded_alpha = lround(SkColorGetA(color) * clamped_alpha);

    return SkColorSetA(color, (U8CPU) rounded_alpha);
}

GraphicsContextState::GraphicsContextState() {
    stroke_flags_.setStyle(SkPaint::kStroke_Style);
    stroke_flags_.setStrokeWidth(SkFloatToScalar(stroke_data_.Thickness()));
    stroke_flags_.setStrokeCap(SkPaint::kDefault_Cap);
    stroke_flags_.setStrokeJoin(SkPaint::kDefault_Join);
    stroke_flags_.setStrokeMiter(SkFloatToScalar(stroke_data_.MiterLimit()));
    stroke_flags_.setFilterQuality(FilterQualityForPaint(interpolation_quality_));
    stroke_flags_.setAntiAlias(should_antialias_);
    fill_flags_.setStyle(SkPaint::kFill_Style);
    fill_flags_.setFilterQuality(FilterQualityForPaint(interpolation_quality_));
    fill_flags_.setAntiAlias(should_antialias_);
    image_flags_.setStyle(SkPaint::kFill_Style);
    image_flags_.setAntiAlias(should_antialias_);
}

GraphicsContextState::GraphicsContextState(const GraphicsContextState &other)
        : fill_flags_(other.fill_flags_),
          stroke_flags_(other.stroke_flags_),
          image_flags_(other.image_flags_) {
    m_fillStyle = other.m_fillStyle;
    m_strokeStyle = other.m_strokeStyle;
    m_fillStyleDirty = true;
    m_strokeStyleDirty = true;
    m_shadowColor = other.m_shadowColor;
    m_shadowBlur = other.m_shadowBlur;
    m_shadowOffset = other.m_shadowOffset;

    m_lineDash = other.m_lineDash;
    m_lineDashOffset = other.m_lineDashOffset;
    m_lineDashDirty = other.m_lineDashDirty;

    m_blendMode = other.m_blendMode;
    fill_flags_.setBlendMode(m_blendMode);
    stroke_flags_.setBlendMode(m_blendMode);

    m_globalAlpha = other.m_globalAlpha;

    m_isTransformInvertible = other.m_isTransformInvertible;
    m_transform = other.m_transform;

    stroke_data_ = other.stroke_data_;
    text_drawing_mode_ = other.text_drawing_mode_;
    interpolation_quality_ = other.interpolation_quality_;
    should_antialias_ = other.should_antialias_;
}

void GraphicsContextState::Copy(const GraphicsContextState &source) {
    this->~GraphicsContextState();
    new(this) GraphicsContextState(source);
}

SkDrawLooper *GraphicsContextState::emptyDrawLooper() const {
    if (!empty_draw_looper_)
        empty_draw_looper_ = SkLayerDrawLooper::Builder().detach();

    return empty_draw_looper_.get();
}

enum ShadowTransformMode {
    kShadowRespectsTransforms,
    kShadowIgnoresTransforms
};
enum ShadowAlphaMode {
    kShadowRespectsAlpha, kShadowIgnoresAlpha
};

inline void addShadowToBuilder(SkLayerDrawLooper::Builder &builder,
                               const FloatSize &offset,
                               float blur,
                               const SkColor color,
                               ShadowTransformMode shadow_transform_mode,
                               ShadowAlphaMode shadow_alpha_mode) {

    // Detect when there's no effective shadow.
    if (!SkColorGetA(color))
        return;

    SkColor sk_color = (color & 0x00FFFFFF) | 0xFF000000;

    SkLayerDrawLooper::LayerInfo info;

    switch (shadow_alpha_mode) {
        case kShadowRespectsAlpha:
            info.fColorMode = SkBlendMode::kDst;
            break;
        case kShadowIgnoresAlpha:
            info.fColorMode = SkBlendMode::kSrc;
            break;
        default:
            return;
    }

    if (blur)
        info.fPaintBits |= SkLayerDrawLooper::kMaskFilter_Bit;  // our blur
    info.fPaintBits |= SkLayerDrawLooper::kColorFilter_Bit;
    info.fOffset.set(offset.width, offset.height);
    info.fPostTranslate = (shadow_transform_mode == kShadowIgnoresTransforms);

    SkPaint *paint = builder.addLayerOnTop(info);

    if (blur) {
        const SkScalar sigma = SkBlurRadiusToSigma(blur);
        const bool respectCTM = shadow_transform_mode != kShadowIgnoresTransforms;
        paint->setMaskFilter(
                SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, sigma, respectCTM));
    }

    paint->setColorFilter(SkColorFilter::MakeModeFilter(sk_color, SkBlendMode::kSrcIn));
}

SkDrawLooper *GraphicsContextState::shadowOnlyDrawLooper() const {
    if (!shadow_only_draw_looper_) {
        SkLayerDrawLooper::Builder builder;

        addShadowToBuilder(builder, m_shadowOffset, m_shadowBlur,
                           m_shadowColor,
                           kShadowIgnoresTransforms,
                           kShadowRespectsAlpha);

        shadow_only_draw_looper_ = builder.detach();
    }
    return shadow_only_draw_looper_.get();
}

SkDrawLooper *GraphicsContextState::shadowAndForegroundDrawLooper() const {
    if (!shadow_and_foreground_draw_looper_) {
        SkLayerDrawLooper::Builder builder;

        addShadowToBuilder(builder, m_shadowOffset, m_shadowBlur,
                           m_shadowColor,
                           kShadowIgnoresTransforms,
                           kShadowRespectsAlpha);
        SkLayerDrawLooper::LayerInfo info;
        builder.addLayerOnTop(info);
        shadow_and_foreground_draw_looper_ = builder.detach();
    }
    return shadow_and_foreground_draw_looper_.get();
}

inline int AlphaChannel(unsigned color) {
    return (color >> 24) & 0xFF;
}

bool GraphicsContextState::shouldDrawShadows() const {
    return AlphaChannel(m_shadowColor) &&
            (m_shadowBlur || !m_shadowOffset.equals(FloatSize::ZERO));
}

// Shadow. (This will need tweaking if we use draw loopers for other things.)
void GraphicsContextState::SetDrawLooper(sk_sp<SkDrawLooper> draw_looper) {
    // Grab a new ref for stroke.
    stroke_flags_.setLooper(draw_looper);
    // Pass the existing ref to fill (to minimize refcount churn).
    fill_flags_.setLooper(std::move(draw_looper));
}

void GraphicsContextState::setFillStyle(const CanvasStyle &fillStyle) {
    m_fillStyle = fillStyle;
    m_fillStyleDirty = true;
}

void GraphicsContextState::updateFillStyle() const {
    if (!m_fillStyleDirty) {
        return;
    }


    m_fillStyle.applyToPaint(fill_flags_);
    fill_flags_.setColor(ScaleAlpha(ColorToSkColor(m_fillStyle.getColor()), m_globalAlpha));
    m_fillStyleDirty = false;
}

void GraphicsContextState::setStrokeStyle(const CanvasStyle &strokeStyle) {
    m_strokeStyle = strokeStyle;
    m_strokeStyleDirty = true;
}

void GraphicsContextState::updateStrokeStyle() const {
    if (!m_strokeStyleDirty) {
        return;
    }

    if (m_strokeStyle.isColorStyle()) {
        stroke_flags_.setColor(ScaleAlpha(ColorToSkColor(m_strokeStyle.getColor()), m_globalAlpha));
    }

    m_strokeStyle.applyToPaint(stroke_flags_);
    m_strokeStyleDirty = false;
}

void GraphicsContextState::setLineCap(LineCap lineCap) {
    SkPaint::Cap cap;
    switch (lineCap) {
        case ButtCap:
            cap = SkPaint::Cap::kButt_Cap;
            break;
        case RoundCap:
            cap = SkPaint::Cap::kRound_Cap;
            break;
        case SquareCap:
            cap = SkPaint::Cap::kSquare_Cap;
            break;
        default:
            return;
    }

    stroke_data_.SetLineCap(cap);
    stroke_flags_.setStrokeCap(cap);
}

void GraphicsContextState::setLineJoin(LineJoin lineJoin) {
    SkPaint::Join join;
    switch (lineJoin) {
        case MiterJoin:
            join = SkPaint::Join::kMiter_Join;
            break;
        case RoundJoin:
            join = SkPaint::Join::kRound_Join;
            break;
        case BevelJoin:
            join = SkPaint::Join::kBevel_Join;
            break;
        default:
            return;
    }

    stroke_data_.SetLineJoin(join);
    stroke_flags_.setStrokeJoin(join);
}

void GraphicsContextState::setStrokeThickness(float thickness) {
    stroke_data_.SetThickness(thickness);
    stroke_flags_.setStrokeWidth(SkFloatToScalar(thickness));
}

void GraphicsContextState::setLineMiterLimit(float limit) {
    stroke_data_.SetMiterLimit(limit);
    stroke_flags_.setStrokeMiter(SkFloatToScalar(limit));
}

void GraphicsContextState::SetLineDash(const DashArray &dashes,
                                       float dash_offset) {
    stroke_data_.SetLineDash(dashes, dash_offset);
}

void GraphicsContextState::setGlobalAlpha(float alpha) {
    m_globalAlpha = std::max(0.0f, std::min(alpha, 1.0f));
    m_fillStyleDirty = true;
    m_strokeStyleDirty = true;
    image_flags_.setColor(SkColorSetA(SK_ColorBLACK, static_cast<U8CPU>(
            lround(0xFF * m_globalAlpha))));
}

SkBlendMode GraphicsContextState::blendMode() const {
    return m_blendMode;
}

void GraphicsContextState::setBlendMode(SkBlendMode blendMode) {
    if (m_blendMode != blendMode) {
        m_blendMode = blendMode;
        fill_flags_.setBlendMode(blendMode);
        stroke_flags_.setBlendMode(blendMode);
        image_flags_.setBlendMode(blendMode);
    }
}

SkColor GraphicsContextState::shadowColor() const {
    return m_shadowColor;
}

void GraphicsContextState::setShadowColor(const Color4F &shadowColor) {
    m_shadowColor = ColorToSkColor(shadowColor);
}

void GraphicsContextState::setShadowBlur(float shadowBlur) {
    m_shadowBlur = shadowBlur;
}

void GraphicsContextState::setShadowOffset(const FloatSize &shadowOffset) {
    m_shadowOffset = shadowOffset;
}

void GraphicsContextState::SetInterpolationQuality(InterpolationQuality quality) {
    interpolation_quality_ = quality;
    stroke_flags_.setFilterQuality(FilterQualityForPaint(quality));
    fill_flags_.setFilterQuality(FilterQualityForPaint(quality));
    image_flags_.setFilterQuality(FilterQualityForPaint(quality));
}

const SkPaint &GraphicsContextState::StrokeFlags(
        const int stroked_path_length,
        const int dash_thickness) const {
    if (m_strokeStyleDirty) {
        updateStrokeStyle();
    }

    stroke_data_.SetupPaintDashPathEffect(&stroke_flags_, stroked_path_length,
                                          dash_thickness);
    return stroke_flags_;
}

const SkPaint &GraphicsContextState::FillFlags() const {
    if (m_fillStyleDirty) {
        updateFillStyle();
    }

    return fill_flags_;
}

const SkPaint &GraphicsContextState::ImageFlags() const {
    return image_flags_;
}

void GraphicsContextState::SetShouldAntialias(bool should_antialias) {
    should_antialias_ = should_antialias;
    stroke_flags_.setAntiAlias(should_antialias);
    fill_flags_.setAntiAlias(should_antialias);
    image_flags_.setAntiAlias(should_antialias);
}

}

