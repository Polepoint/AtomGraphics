//
// Created by neo on 2018/9/2.
//

#include "CanvasRenderingContext2DState.h"

namespace AtomGraphics {

CanvasRenderingContext2DState::CanvasRenderingContext2DState()
        : strokeStyle(Color4F::BLACK),
          fillStyle(Color4F::BLACK),
          fillRule(WindRule::NonZero),
          lineWidth(1),
          lineCap(ButtCap),
          lineJoin(MiterJoin),
          miterLimit(10),
          shadowBlur(0),
          shadowColor(Color4F::TRANSPARENT),
          globalAlpha(1),
          globalComposite(CompositeSourceOver),
          globalBlend(BlendMode::Normal),
          hasInvertibleTransform(true),
          lineDashOffset(0),
          imageSmoothingEnabled(true),
          imageSmoothingQuality(ImageSmoothingQuality::Low),
          textAlign(StartTextAlign),
          textBaseline(AlphabeticTextBaseline),
          textDirection(TextDirection::LTR),
          imageInterpolationQuality(InterpolationDefault) {

}

CanvasRenderingContext2DState::CanvasRenderingContext2DState(const CanvasRenderingContext2DState &other)
        : strokeStyle(other.strokeStyle),
          fillStyle(other.fillStyle),
          fillRule(other.fillRule),
          lineWidth(other.lineWidth),
          lineCap(other.lineCap),
          lineJoin(other.lineJoin),
          miterLimit(other.miterLimit),
          shadowOffset(other.shadowOffset),
          shadowBlur(other.shadowBlur),
          shadowColor(other.shadowColor),
          globalAlpha(other.globalAlpha),
          globalComposite(other.globalComposite),
          globalBlend(other.globalBlend),
          transform(other.transform),
          hasInvertibleTransform(other.hasInvertibleTransform),
          lineDashOffset(other.lineDashOffset),
          imageSmoothingEnabled(other.imageSmoothingEnabled),
          imageSmoothingQuality(other.imageSmoothingQuality),
          textAlign(other.textAlign),
          textBaseline(other.textBaseline),
          textDirection(other.textDirection),
          font(other.font),
          imageInterpolationQuality(other.imageInterpolationQuality) {

}

CanvasRenderingContext2DState &CanvasRenderingContext2DState::operator=(const CanvasRenderingContext2DState &other) {
    if (this == &other)
        return *this;

    strokeStyle = other.strokeStyle;
    fillStyle = other.fillStyle;
    fillRule = other.fillRule;
    lineWidth = other.lineWidth;
    lineCap = other.lineCap;
    lineJoin = other.lineJoin;
    miterLimit = other.miterLimit;
    shadowOffset = other.shadowOffset;
    shadowBlur = other.shadowBlur;
    shadowColor = other.shadowColor;
    globalAlpha = other.globalAlpha;
    globalComposite = other.globalComposite;
    globalBlend = other.globalBlend;
    transform = other.transform;
    hasInvertibleTransform = other.hasInvertibleTransform;
    imageSmoothingEnabled = other.imageSmoothingEnabled;
    imageSmoothingQuality = other.imageSmoothingQuality;
    textAlign = other.textAlign;
    textBaseline = other.textBaseline;
    textDirection = other.textDirection;
    font = other.font;
    imageInterpolationQuality = other.imageInterpolationQuality;

    return *this;
}

#if PLATFORM(ANDROID)

void CanvasRenderingContext2DState::ClipPath(const SkPath &path) {
    clip_list_.ClipPath(path, transform);
}

#endif

}