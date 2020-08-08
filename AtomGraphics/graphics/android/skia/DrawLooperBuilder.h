//
// Created by neo on 2019/1/4.
//

#ifndef ATOMGRAPHICS_DRAWLOOPERBUILDER_H
#define ATOMGRAPHICS_DRAWLOOPERBUILDER_H

#include <skia/effects/SkLayerDrawLooper.h>
#include "base/AtomTypes.h"
#include "math/FloatSize.h"

namespace AtomGraphics {

class DrawLooperBuilder final {
public:
    enum ShadowTransformMode {
        kShadowRespectsTransforms,
        kShadowIgnoresTransforms
    };
    enum ShadowAlphaMode {
        kShadowRespectsAlpha, kShadowIgnoresAlpha
    };

    DrawLooperBuilder();

    ~DrawLooperBuilder();

    // Creates the SkDrawLooper and passes ownership to the caller. The builder
    // should not be used any more after calling this method.
    sk_sp<SkDrawLooper> DetachDrawLooper();

    void AddUnmodifiedContent();

    void AddShadow(const FloatSize &offset,
                   float blur,
                   const Color4F &,
                   ShadowTransformMode = kShadowRespectsTransforms,
                   ShadowAlphaMode = kShadowRespectsAlpha);

private:
    SkLayerDrawLooper::Builder sk_draw_looper_builder_;
};

}


#endif //ATOMGRAPHICS_DRAWLOOPERBUILDER_H
