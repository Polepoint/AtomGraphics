//
// Created by neo on 2018/9/2.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTEXTSKIA_H
#define ATOMGRAPHICS_GRAPHICSCONTEXTSKIA_H

#include <android/bitmap.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <stack>
#include <skia/core/SkPictureRecorder.h>
#include <skia/core/SkPicture.h>
#include "graphics/Pattern.h"
#include "graphics/Gradient.h"
#include "base/AtomTypes.h"
#include "graphics/GraphicsContext.h"
#include "graphics/Path.h"
#include "GraphicsContextState.h"
#include "DrawLooperBuilder.h"

namespace AtomGraphics {

class GraphicsContextSkia final : public GraphicsContext {

public:

    GraphicsContextSkia(SkCanvas *canvas);

    ~GraphicsContextSkia() override;

    void setFillStyle(const CanvasStyle &style) override;

    void setStrokeStyle(const CanvasStyle &style) override;

    void setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor) override;

    void setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor, ShadowMode);

    void setLineCap(const LineCap lineCap) override;

    void setLineJoin(const LineJoin lineJoin) override;

    void setLineDash(const std::vector<float> &dash, float dashOffset) override;

    void setStrokeThickness(float thickness) override;

    void setMiterLimit(float miterLimit) override;

    void fillRect(const FloatRect &rect) override;

    void strokeRect(const FloatRect &rect, float lineWidth) override;

    void clearRect(const FloatRect &rect) override;

    void fillPath(const Path &path) override;

    void strokePath(Path *path) override;

    void strokePath(const Path &,
                    const int length = 0,
                    const int dash_thickness = 0);

    void clipPath(Path *path, WindRule WindRule) override;

    void clipRect(const FloatRect &rect) override;

    void scale(float sx, float sy) override;

    void rotate(float angleInRadians) override;

    void translate(float x, float y) override;

    void concat(const AffineTransform &ctm) override;

    AffineTransform getTransform() override;

    void setTransform(const AffineTransform &transform) override;

    void drawText(const base::String &text, float x, float y, const Font &font, bool fill) override;

    void SetImageInterpolationQuality(InterpolationQuality quality) {
        MutableState()->SetInterpolationQuality(quality);
    }

    InterpolationQuality ImageInterpolationQuality() const {
        return ImmutableState()->GetInterpolationQuality();
    }

    void drawImage(Image *image, const FloatRect &destRect, const FloatRect &srcRect,
                   CompositeOperator compositeOperator, BlendMode blendMode) override;

    void setGlobalAlpha(float alpha) override;

    void setCompositeOperation(CompositeOperator, BlendMode = BlendMode::Normal) override;

    void SetShouldAntialias(bool antialias) {
        MutableState()->SetShouldAntialias(antialias);
    }

    bool ShouldAntialias() const { return ImmutableState()->ShouldAntialias(); }

    void SetTextDrawingMode(TextDrawingMode mode) {
        MutableState()->setTextDrawingMode(mode);
    }

    TextDrawingMode TextDrawingMode() const {
        return ImmutableState()->textDrawingMode();
    }

    void save() override;

    int saveCount() const;

    void restore() override;

    void restoreToSaveCount(int saveCount);

    void clear(const Color4F &color);

    void BeginLayer(float opacity = 1.0f,
                    SkBlendMode = SkBlendMode::kSrcOver,
                    const FloatRect * = nullptr);

    void EndLayer();

    void SaveLayer(const SkRect *bounds, const SkPaint *paint);

    void RestoreLayer();

    void flush() override;

    void setImageInterpolationQuality(InterpolationQuality quality) override;

    SkCanvas *getSkCanvas();

    void resetSkCanvas(SkCanvas *canvas);

    bool getDeviceClipBounds(IntRect *rect);

private:

    SkCanvas *canvas_{nullptr};

    // Paint states stack. The state controls the appearance of drawn content, so
    // this stack enables local drawing state changes with save()/restore() calls.
    // We do not delete from this stack to avoid memory churn.
    std::vector<std::unique_ptr<GraphicsContextState>> paint_state_stack_;

    // Raw pointer to the current state.
    GraphicsContextState *paint_state_{nullptr};

    // Current index on the stack. May not be the last thing on the stack.
    unsigned paint_state_index_{0};

//    std::stack<GraphicsContextState *> m_stateStack;

    const GraphicsContextState *ImmutableState() const { return paint_state_; }

    GraphicsContextState *MutableState() {
        RealizePaintSave();
        return paint_state_;
    }

    void RealizePaintSave() {
        if (!canvas_)
            return;

        if (paint_state_->SaveCount()) {
            paint_state_->DecrementSaveCount();
            ++paint_state_index_;
            if (paint_state_stack_.size() == paint_state_index_) {
                paint_state_stack_.push_back(
                        GraphicsContextState::CreateAndCopy(*paint_state_));
                paint_state_ = paint_state_stack_[paint_state_index_].get();
            } else {
                GraphicsContextState *prior_paint_state = paint_state_;
                paint_state_ = paint_state_stack_[paint_state_index_].get();
                paint_state_->Copy(*prior_paint_state);
            }
        }
    }

    void resetTransform();

    enum DrawType {
        kClipFill,  // Fill that is already known to cover the current clip
        kUntransformedUnclippedFill
    };

    SkCanvas *drawingCanvas();

    bool ContextDisabled() const { return !canvas_; }

    void drawRect(const SkRect &rect, const SkPaint &paint);

    SkFilterQuality ComputeFilterQuality(const FloatRect &dest, const FloatRect &src) const;

    void SetDrawLooper(sk_sp<SkDrawLooper> draw_looper);

    void DrawPath(const SkPath &path, const SkPaint &flags);
};

}


#endif //ATOMGRAPHICS_GRAPHICSCONTEXTSKIA_H
