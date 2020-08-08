//
// Created by neo on 2019/1/4.
//

#include <skia/core/SkColorFilter.h>
#include <skia/core/SkMaskFilter.h>
#include "DrawLooperBuilder.h"
#include "SkiaUtils.h"

namespace AtomGraphics {

DrawLooperBuilder::DrawLooperBuilder() = default;

DrawLooperBuilder::~DrawLooperBuilder() = default;

sk_sp<SkDrawLooper> DrawLooperBuilder::DetachDrawLooper() {
    return sk_draw_looper_builder_.detach();
}

void DrawLooperBuilder::AddUnmodifiedContent() {
    SkLayerDrawLooper::LayerInfo info;
    sk_draw_looper_builder_.addLayerOnTop(info);
}

void DrawLooperBuilder::AddShadow(const FloatSize &offset,
                                  float blur,
                                  const Color4F &color,
                                  ShadowTransformMode shadow_transform_mode,
                                  ShadowAlphaMode shadow_alpha_mode) {

    // Detect when there's no effective shadow.
    if (!color.a)
        return;

    SkLayerDrawLooper::LayerInfo info;

    switch (shadow_alpha_mode) {
        case kShadowRespectsAlpha:
            info.fColorMode = SkBlendMode::kDst;
            break;
        case kShadowIgnoresAlpha:
            info.fColorMode = SkBlendMode::kSrc;
            break;
        default:
            break;
    }

    if (blur)
        info.fPaintBits |= SkLayerDrawLooper::kMaskFilter_Bit;  // our blur
    info.fPaintBits |= SkLayerDrawLooper::kColorFilter_Bit;
    info.fOffset.set(offset.width, offset.height);
    info.fPostTranslate = (shadow_transform_mode == kShadowIgnoresTransforms);

    SkPaint *paint = sk_draw_looper_builder_.addLayerOnTop(info);

    if (blur) {
        const SkScalar sigma = SkBlurRadiusToSigma(blur);
        const bool respectCTM = shadow_transform_mode != kShadowIgnoresTransforms;
        paint->setMaskFilter(
                SkMaskFilter::MakeBlur(kNormal_SkBlurStyle, sigma, respectCTM));
    }

    paint->setColorFilter(
            SkColorFilter::MakeModeFilter(ColorToSkColor(color), SkBlendMode::kSrcIn));
}

}