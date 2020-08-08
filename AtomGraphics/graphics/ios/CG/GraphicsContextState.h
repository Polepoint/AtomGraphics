//
// Created by neo on 2020/4/17.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTEXTSTATE_H
#define ATOMGRAPHICS_GRAPHICSCONTEXTSTATE_H

#include "platform/AtomPlatformConfig.h"
#include "GraphicsTypes.h"
#include "base/AtomTypes.h"
#include "Pattern.h"
#include "Gradient.h"

namespace AtomGraphics {

class GraphicsContext;

struct GraphicsContextState {

    GraphicsContextState()
            : shouldAntialias(true),
              shouldSmoothFonts(true),
              shouldSubpixelQuantizeFonts(true),
              shadowsIgnoreTransforms(false),
            // Core Graphics incorrectly renders shadows with radius > 8px (<rdar://problem/8103442>),
            // but we need to preserve this buggy behavior for canvas and -webkit-box-shadow.
              shadowsUseLegacyRadius(false),
              drawLuminanceMask(false) {
    }

    std::shared_ptr<Gradient> strokeGradient;
    std::shared_ptr<Pattern> strokePattern;

    std::shared_ptr<Gradient> fillGradient;
    std::shared_ptr<Pattern> fillPattern;

    FloatSize shadowOffset;

    float strokeThickness{0};
    float shadowBlur{0};

    TextDrawingMode textDrawingMode{TextDrawingMode::kTextModeFill};

    Color4F strokeColor{Color4F::BLACK};
    Color4F fillColor{Color4F::BLACK};
    Color4F shadowColor;

    StrokeStyle strokeStyle{kSolidStroke};
    WindRule fillRule{WindRule::NonZero};

    float alpha{1};
    CompositeOperator compositeOperator{CompositeOperator::CompositeSourceOver};
    BlendMode blendMode{BlendMode::Normal};
    InterpolationQuality imageInterpolationQuality{InterpolationQuality::InterpolationDefault};

    bool shouldAntialias : 1;
    bool shouldSmoothFonts : 1;
    bool shouldSubpixelQuantizeFonts : 1;
    bool shadowsIgnoreTransforms : 1;
    bool shadowsUseLegacyRadius : 1;
    bool drawLuminanceMask : 1;
};

};


#endif //ATOMGRAPHICS_GRAPHICSCONTEXTSTATE_H
