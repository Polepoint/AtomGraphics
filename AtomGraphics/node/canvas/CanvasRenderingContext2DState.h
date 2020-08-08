//
// Created by neo on 2018/9/2.
//

#ifndef ATOMGRAPHICS_CANVASSTATE_H
#define ATOMGRAPHICS_CANVASSTATE_H

#include "CanvasStyle.h"
#include "CanvasTypes.h"
#include "base/AtomTypes.h"
#include "math/AtomGeometry.h"
#include "graphics/AffineTransform.h"
#include "graphics/font/Font.h"
#include "graphics/GraphicsTypes.h"
#include "platform/AtomPlatformConfig.h"
#include <vector>

#if PLATFORM(ANDROID)

#include <skia/core/SkPath.h>
#include "node/canvas/android/skia/ClipList.h"

#endif

namespace AtomGraphics {

struct CanvasRenderingContext2DState final {

    CanvasRenderingContext2DState();

    CanvasRenderingContext2DState(const CanvasRenderingContext2DState &other);

    CanvasRenderingContext2DState &operator=(const CanvasRenderingContext2DState &other);

    CanvasStyle strokeStyle;
    CanvasStyle fillStyle;
    WindRule fillRule;
    float lineWidth;
    LineCap lineCap;
    LineJoin lineJoin;
    float miterLimit;
    FloatSize shadowOffset;
    float shadowBlur;
    Color4F shadowColor;
    float globalAlpha;
    CompositeOperator globalComposite;
    BlendMode globalBlend;
    AffineTransform transform;
    bool hasInvertibleTransform;
    std::vector<float> lineDash;
    float lineDashOffset;
    bool imageSmoothingEnabled;
    ImageSmoothingQuality imageSmoothingQuality;

    InterpolationQuality imageInterpolationQuality;

    // Text state.
    TextAlign textAlign;
    TextBaseline textBaseline;
    TextDirection textDirection;

    std::string unparsedFont;
    Font font;

#if PLATFORM(ANDROID)
    bool hasComplexClip{false};

    ClipList clip_list_;

    enum PaintType {
        kFillPaintType,
        kStrokePaintType,
        kImagePaintType,
    };

    void ClipPath(const SkPath &path);

    void PlaybackClips(SkCanvas *canvas) const {
        clip_list_.Playback(canvas);
    }

#endif
};

}


#endif //ATOMGRAPHICS_CANVASSTATE_H
