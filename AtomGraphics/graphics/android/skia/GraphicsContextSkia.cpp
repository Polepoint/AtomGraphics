//
// Created by neo on 2018/9/2.
//

#include <cmath>
#include <skia/core/SkPath.h>
#include <skia/core/SkSurface.h>
#include <skia/gpu/GrContext.h>
#include <skia/gpu/gl/GrGLInterface.h>
#include <skia/core/SkTextBlob.h>
#include "graphics/font/PaintTextBlobBuilder.h"
#include "graphics/font/ShapeResultBloberizer.h"
#include "graphics/font/WordShaper.h"
#include "base/AtomLog.h"
#include "base/AtomTypes.h"
#include "math/AtomGeometry.h"
#include "node/canvas/CanvasTypes.h"
#include "node/canvas/CanvasStyle.h"
#include "graphics/Path.h"
#include "graphics/AffineTransform.h"
#include "graphics/GraphicsContext.h"
#include "GraphicsContextSkia.h"
#include "GraphicsContextState.h"
#include "DrawLooperBuilder.h"
#include "SkiaUtils.h"


namespace AtomGraphics {

GraphicsContextSkia::GraphicsContextSkia(SkCanvas *canvas) : canvas_(canvas) {
    paint_state_stack_.push_back(GraphicsContextState::Create());
    paint_state_ = paint_state_stack_.back().get();
}

GraphicsContextSkia::~GraphicsContextSkia() = default;

void GraphicsContextSkia::setFillStyle(const CanvasStyle &style) {
    MutableState()->setFillStyle(style);
}

void GraphicsContextSkia::setStrokeStyle(const CanvasStyle &style) {
    MutableState()->setStrokeStyle(style);
}

void GraphicsContextSkia::setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor) {
    setShadow(offset, blurRadius, shadowColor, kDrawShadowAndForeground);
}

void GraphicsContextSkia::setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor,
                                    ShadowMode shadow_mode) {
    if (!canvas_)
        return;

    DrawLooperBuilder draw_looper_builder;
    if (!shadowColor.a) {
        // When shadow-only but there is no shadow, we use an empty draw looper
        // to disable rendering of the source primitive.  When not shadow-only, we
        // clear the looper.
        SetDrawLooper(shadow_mode != kDrawShadowOnly
                      ? nullptr
                      : draw_looper_builder.DetachDrawLooper());
        return;
    }

    draw_looper_builder.AddShadow(offset, blurRadius,
                                  shadowColor,
                                  DrawLooperBuilder::kShadowRespectsTransforms,
                                  DrawLooperBuilder::kShadowRespectsAlpha);
    if (shadow_mode == kDrawShadowAndForeground) {
        draw_looper_builder.AddUnmodifiedContent();
    }
    SetDrawLooper(draw_looper_builder.DetachDrawLooper());
}

void GraphicsContextSkia::setLineCap(const LineCap lineCap) {
    MutableState()->setLineCap(lineCap);
}

void GraphicsContextSkia::setLineJoin(const LineJoin lineJoin) {
    MutableState()->setLineJoin(lineJoin);
}

void GraphicsContextSkia::setStrokeThickness(float thickness) {
    MutableState()->setStrokeThickness(thickness);
}

void GraphicsContextSkia::setMiterLimit(float miterLimit) {
    MutableState()->setLineMiterLimit(miterLimit);
}

void GraphicsContextSkia::setLineDash(const std::vector<float> &dash, float dashOffset) {
    MutableState()->SetLineDash(dash, dashOffset);
}

void GraphicsContextSkia::drawRect(const SkRect &rect, const SkPaint &paint) {
    auto ctx = drawingCanvas();
    if (!ctx)
        return;

    ctx->drawRect(rect, paint);
}

void GraphicsContextSkia::fillRect(const FloatRect &rect) {
    auto ctx = drawingCanvas();
    if (!ctx)
        return;

    drawRect(rect, ImmutableState()->FillFlags());
}

void GraphicsContextSkia::strokeRect(const FloatRect &rect, float line_width) {
    if (ContextDisabled())
        return;

    SkPaint flags(ImmutableState()->StrokeFlags());
    flags.setStrokeWidth(WebCoreFloatToSkScalar(line_width));
    // Reset the dash effect to account for the width
    ImmutableState()->GetStrokeData().SetupPaintDashPathEffect(&flags);
    // strokerect has special rules for CSS when the rect is degenerate:
    // if width==0 && height==0, do nothing
    // if width==0 || height==0, then just draw line for the other dimension
    SkRect r(rect);
    bool valid_w = r.width() > 0;
    bool valid_h = r.height() > 0;
    if (valid_w && valid_h) {
        drawRect(r, flags);
    } else if (valid_w || valid_h) {
        // we are expected to respect the lineJoin, so we can't just call
        // drawLine -- we have to create a path that doubles back on itself.
        SkPath path;
        path.moveTo(r.fLeft, r.fTop);
        path.lineTo(r.fRight, r.fBottom);
        path.close();
        DrawPath(path, flags);
    }
}

void GraphicsContextSkia::clearRect(const FloatRect &rect) {

    SkCanvas *c = drawingCanvas();
    if (!c)
        return;
    if (!ImmutableState()->isTransformInvertible())
        return;

    SkIRect clip_bounds;
    if (!c->getDeviceClipBounds(&clip_bounds))
        return;

    SkPaint clearPaint;
    clearPaint.setBlendMode(SkBlendMode::kClear);
    clearPaint.setStyle(SkPaint::kFill_Style);


    c->drawRect(rect, clearPaint);
}


void GraphicsContextSkia::DrawPath(const SkPath &path, const SkPaint &flags) {
    if (ContextDisabled())
        return;

    canvas_->drawPath(path, flags);
}

void GraphicsContextSkia::fillPath(const Path &path_to_fill) {
    if (ContextDisabled() || path_to_fill.isEmpty())
        return;

    DrawPath(path_to_fill.getSkPath(), ImmutableState()->FillFlags());
}

void GraphicsContextSkia::strokePath(Path *path) {
    if (ContextDisabled() || path->isEmpty())
        return;

    strokePath(*path, 0, 0);
}

void GraphicsContextSkia::strokePath(const Path &path_to_stroke,
                                     const int length,
                                     const int dash_thickness) {
    DrawPath(path_to_stroke.getSkPath(), ImmutableState()->StrokeFlags(length, dash_thickness));
}

void GraphicsContextSkia::clipPath(Path *path, WindRule windRule) {
    if (ContextDisabled())
        return;

    SkPath skPath(path->getSkPath());
    SkPath::FillType fillType =
            windRule == WindRule::NonZero ? SkPath::FillType::kWinding_FillType : SkPath::FillType::kEvenOdd_FillType;
    skPath.setFillType(fillType);
    canvas_->clipPath(skPath);
}

void GraphicsContextSkia::clipRect(const FloatRect &rect) {
    SkCanvas *c = drawingCanvas();
    if (!c) {
        return;
    }
    c->clipRect(rect);
}

void GraphicsContextSkia::scale(float sx, float sy) {
    if (ContextDisabled())
        return;

    canvas_->scale(WebCoreFloatToSkScalar(sx), WebCoreFloatToSkScalar(sy));
}

void GraphicsContextSkia::rotate(float angleInRadians) {
    if (ContextDisabled())
        return;

    canvas_->rotate(static_cast<SkScalar>(angleInRadians * (180.0 / M_PI)));
}

void GraphicsContextSkia::translate(float x, float y) {
    if (ContextDisabled())
        return;

    if (!x && !y)
        return;

    canvas_->translate(WebCoreFloatToSkScalar(x), WebCoreFloatToSkScalar(y));
}

void GraphicsContextSkia::resetTransform() {
    if (ContextDisabled())
        return;

    canvas_->resetMatrix();
}

void GraphicsContextSkia::concat(const AffineTransform &matrix) {
    SkCanvas *c = drawingCanvas();
    if (!c)
        return;

    canvas_->concat(matrix);
}

AffineTransform GraphicsContextSkia::getTransform() {
    return canvas_->getTotalMatrix();
}

void GraphicsContextSkia::setTransform(const AffineTransform &transform) {
    if (ContextDisabled())
        return;

    canvas_->setMatrix(transform);
}

void GraphicsContextSkia::SetDrawLooper(sk_sp<SkDrawLooper> draw_looper) {
    if (ContextDisabled())
        return;

    MutableState()->SetDrawLooper(std::move(draw_looper));
}

void GraphicsContextSkia::drawText(const base::String &text, float x, float y, const Font &font, bool fill) {
    font.drawBidiText(canvas_, text, {x, y}, fill ? ImmutableState()->FillFlags() : ImmutableState()->StrokeFlags());
}

SkFilterQuality GraphicsContextSkia::ComputeFilterQuality(
        const FloatRect &dest,
        const FloatRect &src) const {
    InterpolationQuality resampling = ComputeInterpolationQuality(
            SkScalarToFloat(src.width()), SkScalarToFloat(src.height()),
            SkScalarToFloat(dest.width()), SkScalarToFloat(dest.height()));

    if (resampling == InterpolationNone) {
        // FIXME: This is to not break tests (it results in the filter bitmap flag
        // being set to true). We need to decide if we respect InterpolationNone
        // being returned from computeInterpolationQuality.
        resampling = InterpolationLow;
    }
    return static_cast<SkFilterQuality>(
            std::min(resampling, ImageInterpolationQuality()));
}


void GraphicsContextSkia::drawImage(Image *image, const FloatRect &destRect, const FloatRect &srcRect,
                                    CompositeOperator compositeOperator, BlendMode blendMode) {
    if (!canvas_ || !image)
        return;

    SkPaint image_flags = ImmutableState()->ImageFlags();
    image_flags.setBlendMode(WebCoreCompositeToSkiaComposite(compositeOperator, blendMode));
    image_flags.setFilterQuality(ComputeFilterQuality(destRect, srcRect));
    image_flags.setAntiAlias(ShouldAntialias());

    FloatRect adjusted_src_rect = srcRect;
    adjusted_src_rect.intersect(SkRect::MakeWH(image->width(), image->height()));

    if (adjusted_src_rect.isEmpty() || destRect.isEmpty())
        return;  // Nothing to draw.

    FloatRect adjusted_dst_rect = destRect;
    canvas_->drawImageRect(image->getSkImage(), adjusted_src_rect, adjusted_dst_rect,
                           &image_flags, SkCanvas::kStrict_SrcRectConstraint);
}

void GraphicsContextSkia::setGlobalAlpha(float alpha) {
    MutableState()->setGlobalAlpha(alpha);
}

void GraphicsContextSkia::setCompositeOperation(CompositeOperator compositeOperator, BlendMode blendMode) {
    MutableState()->setBlendMode(WebCoreCompositeToSkiaComposite(compositeOperator, blendMode));
}

void GraphicsContextSkia::save() {
    if (!canvas_)
        return;

    paint_state_->IncrementSaveCount();

    canvas_->save();
}

int GraphicsContextSkia::saveCount() const {
    return paint_state_->SaveCount() + canvas_->getSaveCount();
}

void GraphicsContextSkia::restore() {
    if (!canvas_)
        return;

    if (!paint_state_index_ && !paint_state_->SaveCount()) {
        return;
    }

    if (paint_state_->SaveCount()) {
        paint_state_->DecrementSaveCount();
    } else {
        paint_state_index_--;
        paint_state_ = paint_state_stack_[paint_state_index_].get();
    }

    canvas_->restore();
}

void GraphicsContextSkia::restoreToSaveCount(int saveCount) {
    int currentSaveCount = this->saveCount();
    if (currentSaveCount == saveCount) {
        return;
    }

    if (currentSaveCount > saveCount) {
        while (currentSaveCount > saveCount) {
            restore();
            currentSaveCount--;
        }
    } else {
        while (currentSaveCount < saveCount) {
            save();
            currentSaveCount++;
        }
    }
}

void GraphicsContextSkia::clear(const Color4F &color) {
    canvas_->clear(ColorToSkColor(color));
}

void GraphicsContextSkia::BeginLayer(float opacity,
                                     SkBlendMode xfermode,
                                     const FloatRect *bounds) {
    if (!canvas_)
        return;

    SkPaint layer_paint;
    layer_paint.setAlpha(static_cast<unsigned char>(opacity * 255));
    layer_paint.setBlendMode(xfermode);

    if (bounds) {
        SkRect sk_bounds = *bounds;
        SaveLayer(&sk_bounds, &layer_paint);
    } else {
        SaveLayer(nullptr, &layer_paint);
    }
}

void GraphicsContextSkia::EndLayer() {
    if (!canvas_)
        return;

    RestoreLayer();
}

void GraphicsContextSkia::SaveLayer(const SkRect *bounds, const SkPaint *paint) {
    if (canvas_)
        return;

    canvas_->saveLayer(bounds, paint);
}

void GraphicsContextSkia::RestoreLayer() {
    if (canvas_)
        return;

    canvas_->restore();
}

void GraphicsContextSkia::flush() {
    drawingCanvas()->flush();
}

void GraphicsContextSkia::setImageInterpolationQuality(InterpolationQuality quality) {
    MutableState()->SetInterpolationQuality(quality);
}

SkCanvas *GraphicsContextSkia::drawingCanvas() {
    return canvas_;
}

SkCanvas *GraphicsContextSkia::getSkCanvas() {
    return drawingCanvas();
}

void GraphicsContextSkia::resetSkCanvas(SkCanvas *canvas) {
    canvas_ = canvas;
}

bool GraphicsContextSkia::getDeviceClipBounds(IntRect *rect) {
    SkIRect skIRect;
    bool sk = drawingCanvas()->getDeviceClipBounds(&skIRect);
    *rect = skIRect;
    return sk;
}

}