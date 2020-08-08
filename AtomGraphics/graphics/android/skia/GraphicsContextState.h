//
// Created by neo on 2018/9/4.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTEXTSTATE_H
#define ATOMGRAPHICS_GRAPHICSCONTEXTSTATE_H

#include <skia/core/SkPaint.h>
#include <skia/core/SkDrawLooper.h>
#include <vector>
#include "graphics/DashArray.h"
#include "graphics/AffineTransform.h"
#include "node/canvas/CanvasTypes.h"
#include "node/canvas/CanvasStyle.h"
#include "StrokeData.h"

namespace AtomGraphics {

enum ImageType {
    kNoImage, kOpaqueImage, kNonOpaqueImage
};


class GraphicsContextState {

public:

    GraphicsContextState();

    GraphicsContextState(const GraphicsContextState &other);

    static std::unique_ptr<GraphicsContextState> Create() {
        return std::unique_ptr<GraphicsContextState>(new GraphicsContextState());
    }

    static std::unique_ptr<GraphicsContextState> CreateAndCopy(
            const GraphicsContextState &other) {
        return std::unique_ptr<GraphicsContextState>(new GraphicsContextState(other));
    }

    void Copy(const GraphicsContextState &);

    uint16_t SaveCount() const { return save_count_; }

    void IncrementSaveCount() { ++save_count_; }

    void DecrementSaveCount() { --save_count_; }

    void setFillStyle(const CanvasStyle &fillStyle);

    void setStrokeStyle(const CanvasStyle &strokeStyle);

    void setLineCap(LineCap lineCap);

    void setLineJoin(LineJoin lineJoin);

    void setStrokeThickness(float thickness);

    void setLineMiterLimit(float miterLimit);

    void SetLineDash(const DashArray &, float);

    void setGlobalAlpha(float alpha);

    SkColor shadowColor() const;

    void setShadowColor(const Color4F &shadowColor);

    void setShadowBlur(float shadowBlur);

    void setShadowOffset(const FloatSize &shadowOffset);

    SkBlendMode blendMode() const;

    void setBlendMode(SkBlendMode globalComposite);

    bool shouldDrawShadows() const;

    const AffineTransform &transform() const {
        return m_transform;
    }

    void setTransform(const AffineTransform transform) {
        m_transform = transform;
        m_isTransformInvertible = m_transform.isInvertible();
    }

    bool isTransformInvertible() const {
        return m_isTransformInvertible;
    }

    void SetDrawLooper(sk_sp<SkDrawLooper> draw_looper);

    // Image interpolation control.
    InterpolationQuality GetInterpolationQuality() const {
        return interpolation_quality_;
    }

    void SetInterpolationQuality(InterpolationQuality);


    const SkPaint &StrokeFlags(const int stroked_path_length = 0,
                               const int dash_thickness = 0) const;

    const SkPaint &FillFlags() const;

    const SkPaint &ImageFlags() const;

    bool ShouldAntialias() const { return should_antialias_; }

    void SetShouldAntialias(bool);

    const StrokeData &GetStrokeData() const { return stroke_data_; }

    TextDrawingMode textDrawingMode() const { return text_drawing_mode_; }

    void setTextDrawingMode(TextDrawingMode mode) {
        text_drawing_mode_ = mode;
    }

private:

    mutable SkPaint stroke_flags_;
    mutable SkPaint fill_flags_;
    SkPaint image_flags_;
    StrokeData stroke_data_;
    uint16_t save_count_{0};
    TextDrawingMode text_drawing_mode_{kTextModeFill};
    InterpolationQuality interpolation_quality_{InterpolationDefault};
    bool should_antialias_{true};

    mutable CanvasStyle m_fillStyle;
    mutable CanvasStyle m_strokeStyle;

    mutable bool m_fillStyleDirty{false};
    mutable bool m_strokeStyleDirty{false};

    std::vector<double> m_lineDash;
    float m_lineDashOffset;
    bool m_lineDashDirty{false};

    FloatSize m_shadowOffset;
    float m_shadowBlur;
    SkColor m_shadowColor;
    mutable sk_sp<SkDrawLooper> empty_draw_looper_;
    mutable sk_sp<SkDrawLooper> shadow_only_draw_looper_;
    mutable sk_sp<SkDrawLooper> shadow_and_foreground_draw_looper_;

    AffineTransform m_transform;
    bool m_isTransformInvertible{true};

    float m_globalAlpha{1};

    SkBlendMode m_blendMode{SkBlendMode::kSrcOver};

    void updateFillStyle() const;

    void updateStrokeStyle() const;

    SkDrawLooper *emptyDrawLooper() const;

    SkDrawLooper *shadowOnlyDrawLooper() const;

    SkDrawLooper *shadowAndForegroundDrawLooper() const;
};
}


#endif //ATOMGRAPHICS_GRAPHICSCONTEXTSTATE_H
