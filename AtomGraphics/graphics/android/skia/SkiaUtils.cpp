//
// Created by neo on 2019/1/5.
//

#include <skia/core/SkBlendMode.h>
#include "SkiaUtils.h"

namespace AtomGraphics {

SkPath::FillType WebCoreWindingRuleToSkiaFillType(WindRule windRule) {
    if (windRule == WindRule::NonZero)
        return SkPath::kWinding_FillType;
    if (windRule == WindRule::EvenOdd)
        return SkPath::kEvenOdd_FillType;

    return SkPath::kEvenOdd_FillType;
}


SkBlendMode WebCoreCompositeToSkiaComposite(CompositeOperator op,
                                            BlendMode blend_mode) {
    if (blend_mode != BlendMode::Normal) {
        return WebCoreBlendModeToSkBlendMode(blend_mode);
    }

    switch (op) {
        case CompositeClear:
            return SkBlendMode::kClear;
        case CompositeCopy:
            return SkBlendMode::kSrc;
        case CompositeSourceOver:
            return SkBlendMode::kSrcOver;
        case CompositeSourceIn:
            return SkBlendMode::kSrcIn;
        case CompositeSourceOut:
            return SkBlendMode::kSrcOut;
        case CompositeSourceAtop:
            return SkBlendMode::kSrcATop;
        case CompositeDestinationOver:
            return SkBlendMode::kDstOver;
        case CompositeDestinationIn:
            return SkBlendMode::kDstIn;
        case CompositeDestinationOut:
            return SkBlendMode::kDstOut;
        case CompositeDestinationAtop:
            return SkBlendMode::kDstATop;
        case CompositeXOR:
            return SkBlendMode::kXor;
        case CompositePlusLighter:
            return SkBlendMode::kPlus;
        default:
            //FIXME: unreachable
            return SkBlendMode::kSrcOver;
    }
}

SkBlendMode WebCoreBlendModeToSkBlendMode(BlendMode blend_mode) {
    switch (blend_mode) {
        case BlendMode::Normal:
            return SkBlendMode::kSrcOver;
        case BlendMode::Multiply:
            return SkBlendMode::kMultiply;
        case BlendMode::Screen:
            return SkBlendMode::kScreen;
        case BlendMode::Overlay:
            return SkBlendMode::kOverlay;
        case BlendMode::Darken:
            return SkBlendMode::kDarken;
        case BlendMode::Lighten:
            return SkBlendMode::kLighten;
        case BlendMode::ColorDodge:
            return SkBlendMode::kColorDodge;
        case BlendMode::ColorBurn:
            return SkBlendMode::kColorBurn;
        case BlendMode::HardLight:
            return SkBlendMode::kHardLight;
        case BlendMode::SoftLight:
            return SkBlendMode::kSoftLight;
        case BlendMode::Difference:
            return SkBlendMode::kDifference;
        case BlendMode::Exclusion:
            return SkBlendMode::kExclusion;
        case BlendMode::Hue:
            return SkBlendMode::kHue;
        case BlendMode::Saturation:
            return SkBlendMode::kSaturation;
        case BlendMode::Color:
            return SkBlendMode::kColor;
        case BlendMode::Luminosity:
            return SkBlendMode::kLuminosity;
        default:
            //FIXME: unreachable
            return SkBlendMode::kSrcOver;
    }
}

bool NearlyIntegral(float value) {
    return fabs(value - floorf(value)) < std::numeric_limits<float>::epsilon();
}

InterpolationQuality ComputeInterpolationQuality(float src_width,
                                                 float src_height,
                                                 float dest_width,
                                                 float dest_height) {
    // The percent change below which we will not resample. This usually means
    // an off-by-one error on the web page, and just doing nearest neighbor
    // sampling is usually good enough.
    const float kFractionalChangeThreshold = 0.025f;

    // Images smaller than this in either direction are considered "small" and
    // are not resampled ever (see below).
    const int kSmallImageSizeThreshold = 8;

    // The amount an image can be stretched in a single direction before we
    // say that it is being stretched so much that it must be a line or
    // background that doesn't need resampling.
    const float kLargeStretch = 3.0f;

    // Figure out if we should resample this image. We try to prune out some
    // common cases where resampling won't give us anything, since it is much
    // slower than drawing stretched.
    float diff_width = fabs(dest_width - src_width);
    float diff_height = fabs(dest_height - src_height);
    bool width_nearly_equal = diff_width < std::numeric_limits<float>::epsilon();
    bool height_nearly_equal =
            diff_height < std::numeric_limits<float>::epsilon();
    // We don't need to resample if the source and destination are the same.
    if (width_nearly_equal && height_nearly_equal)
        return InterpolationNone;

    if (src_width <= kSmallImageSizeThreshold ||
            src_height <= kSmallImageSizeThreshold ||
            dest_width <= kSmallImageSizeThreshold ||
            dest_height <= kSmallImageSizeThreshold) {
        // Small image detected.

        // Resample in the case where the new size would be non-integral.
        // This can cause noticeable breaks in repeating patterns, except
        // when the source image is only one pixel wide in that dimension.
        if ((!NearlyIntegral(dest_width) &&
                src_width > 1 + std::numeric_limits<float>::epsilon()) ||
                (!NearlyIntegral(dest_height) &&
                        src_height > 1 + std::numeric_limits<float>::epsilon()))
            return InterpolationLow;

        // Otherwise, don't resample small images. These are often used for
        // borders and rules (think 1x1 images used to make lines).
        return InterpolationNone;
    }

    if (src_height * kLargeStretch <= dest_height ||
            src_width * kLargeStretch <= dest_width) {
        // Large image detected.

        // Don't resample if it is being stretched a lot in only one direction.
        // This is trying to catch cases where somebody has created a border
        // (which might be large) and then is stretching it to fill some part
        // of the page.
        if (width_nearly_equal || height_nearly_equal)
            return InterpolationNone;

        // The image is growing a lot and in more than one direction. Resampling
        // is slow and doesn't give us very much when growing a lot.
        return InterpolationLow;
    }

    if ((diff_width / src_width < kFractionalChangeThreshold) &&
            (diff_height / src_height < kFractionalChangeThreshold)) {
        // It is disappointingly common on the web for image sizes to be off by
        // one or two pixels. We don't bother resampling if the size difference
        // is a small fraction of the original size.
        return InterpolationNone;
    }

    // Everything else gets resampled at default quality.
    return InterpolationDefault;
}

}

