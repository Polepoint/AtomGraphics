//
// Created by neo on 2019/1/4.
//

#ifndef ATOMGRAPHICS_SKIAUTILS_H
#define ATOMGRAPHICS_SKIAUTILS_H

#include <cmath>
#include <skia/core/SkTypes.h>
#include <skia/core/SkScalar.h>
#include <skia/core/SkPath.h>
#include <skia/core/SkBlendMode.h>
#include "node/canvas/CanvasTypes.h"

namespace AtomGraphics {

#define ColorToSkColor(color4F) \
    SkColor4f::Pin(color4F.r, color4F.g, color4F.b, color4F.a).toSkColor()

SkPath::FillType WebCoreWindingRuleToSkiaFillType(WindRule windRule);

SkBlendMode WebCoreCompositeToSkiaComposite(CompositeOperator,
                                            BlendMode = BlendMode::Normal);

SkBlendMode WebCoreBlendModeToSkBlendMode(BlendMode);

// Skia has problems when passed infinite, etc floats, filter them to 0.
inline SkScalar WebCoreFloatToSkScalar(float f) {
    return SkFloatToScalar(std::isfinite(f) ? f : 0);
}


bool NearlyIntegral(float value);

InterpolationQuality ComputeInterpolationQuality(float src_width,
                                                 float src_height,
                                                 float dest_width,
                                                 float dest_height);

// This replicates the old skia behavior when it used to take radius for blur.
// Now it takes sigma.
inline SkScalar SkBlurRadiusToSigma(SkScalar radius) {
    SkASSERT(radius >= 0);
    if (radius == 0)
        return 0.0f;
    return 0.288675f * radius + 0.5f;
}

}


#endif //ATOMGRAPHICS_SKIAUTILS_H
