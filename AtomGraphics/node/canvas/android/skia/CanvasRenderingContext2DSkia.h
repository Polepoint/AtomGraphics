//
// Created by neo on 2018/9/1.
//

#ifndef ATOMGRAPHICS_CANVASRENDERINGCONTEXT2DSKIA_H
#define ATOMGRAPHICS_CANVASRENDERINGCONTEXT2DSKIA_H


#include "node/canvas/CanvasRenderingContext2DState.h"
#include "node/canvas/CanvasRenderingContext2D.h"
#include "CanvasNodeSkia.h"
#include "base/AtomTypes.h"
#include <string>
#include <graphics/android/skia/GraphicsContextState.h>
#include "base/ATString.h"
#include "graphics/font/TextMetrics.h"
#include "graphics/android/ResourceProvider.h"
#include "graphics/android/skia/GraphicsContextSkia.h"

namespace AtomGraphics {

class CanvasNodeSkia;

class CanvasRenderingContext2DSkia final : public CanvasRenderingContext2D {

public:

    CanvasRenderingContext2DSkia(CanvasNode *node);

    void setFillStyle(const CanvasStyle &style) override;

    void setStrokeStyle(const CanvasStyle &style) override;

    void setShadowColor(const Color4F &color) override;

    void setShadowBlur(float blur) override;

    void setShadowOffsetX(float offsetX) override;

    void setShadowOffsetY(float offsetY) override;

    void setLineCap(const LineCap lineCap) override;

    void setLineJoin(const LineJoin lineJoin) override;

    void setLineWidth(float width) override;

    void setMiterLimit(float limit) override;

    void setLineDash(const std::vector<float> &dash) override;

    void setLineDashOffset(float dashOffset) override;

    void rect(float x, float y, float width, float height) override;

    void fillRect(float x, float y, float width, float height) override;

    void strokeRect(float x, float y, float width, float height) override;

    void clearRect(float x, float y, float width, float height) override;

    void fill(WindRule = WindRule::NonZero) override;

    void stroke() override;

    void beginPath() override;

    void moveTo(float x, float y) override;

    void closePath() override;

    void lineTo(float x, float y) override;

    void clip(WindRule windRule) override;

    void quadraticCurveTo(float cpx, float cpy, float x, float y) override;

    void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y,
                       float x, float y) override;

    void arc(float x, float y, float r, float sAngle, float eAngle,
             bool counterclockwise) override;

    void arcTo(float x1, float y1, float x2, float y2, float r) override;

    bool isPointInPath(float x, float y) override;

    void scale(float scaleWidth, float scaleHeight) override;

    void rotate(float angleInRadians) override;

    void translate(float tx, float ty) override;

    void transform(float a, float b, float c, float d, float tx, float ty) override;

    void setTransform(float a, float b, float c, float d, float tx, float ty) override;

    void setFont(const std::string &fontString) override;

    std::string getFont() override;

    void setTextAlign(TextAlign textAlign) override;

    void setTextBaseline(TextBaseline textBaseline) override;

    void fillText(const base::String &text, float x, float y, float *maxWidth) override;

    void strokeText(const base::String &text, float x, float y, float *maxWidth) override;

    TextMetrics measureText(const base::String &text) override;

    void drawImage(CanvasImageSource *image, float sx, float sy, float sWidth, float sHeight,
                   float x, float y, float dWidth, float dHeight) override;

    scoped_refptr<ImageData> getImageData(int x, int y, int width, int height) override;

    void putImageData(ImageData *imageData, int x, int y,
                      int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight) override;

    void setGlobalAlpha(float alpha) override;

    void setGlobalCompositeOperation(const CompositeOperator operation,
                                     const BlendMode blendMode) override;

    void save() override;

    void restore() override;

    void createEvent() override;

    void *toDataURL() override;

    void enableConsumingDrawCache() override;

    void releaseConsumingImage() override;

    void paintInContext(GraphicsContext *context, const FloatRect &contentBounds) override;

    sk_sp<SkImage> GetImage(AccelerationHint hint);

    bool IsAccelerated() const;

    void reset() override;

private:

    long m_hostPageID{-1};

    int m_unrealizedSaveCount{0};

    enum SnapshotState {
        kInitialSnapshotState,
        kDidAcquireSnapshot,
        kDrawnToAfterSnapshot,
    };
    mutable SnapshotState snapshot_state_;

    mutable std::unique_ptr<ResourceProvider> m_resourceProvider;
    mutable SkPictureRecorder m_recorder;

    mutable bool have_recorded_draw_commands_{false};

    mutable std::unique_ptr<GraphicsContextSkia> m_drawingContext;
    std::unique_ptr<Path> m_path;

    std::vector<CanvasRenderingContext2DState> m_stateStack;

    enum DrawType {
        kClipFill,  // Fill that is already known to cover the current clip
        kUntransformedUnclippedFill
    };

    FloatRect m_dirtyRect{FloatRect::ZERO};

    bool m_cacheConsuming{false};
    sk_sp<SkImage> m_consumingDrawCachedImage;

    bool gpu_readback_invoked_in_current_frame_{false};

    int gpu_readback_successive_frames_{0};

    const CanvasRenderingContext2DState &currentState() const;

    CanvasRenderingContext2DState &modifiableState();

    void realizeSaves();

    void realizeSavesLoop();

    GraphicsContextSkia *drawingContext() const;

    void createDrawingContext() const;

    void applyLineDash() const;

    template<typename DrawFunc>
    void CompositedDraw(const DrawFunc &, GraphicsContextSkia *);

    template<typename DrawFunc, typename ContainsFunc>
    bool Draw(const DrawFunc &,
              const ContainsFunc &,
              const FloatRect &bounds,
              const CanvasStyle &style,
              ImageType image_type = ImageType::kNoImage);

    void DidDraw(const IntRect &dirty_rect);

    bool IsFullCanvasCompositeMode(CompositeOperator op);

    void ClearCanvas();

    void CheckOverdraw(const FloatRect &rect, ImageType image_type, DrawType draw_type);

    bool ComputeDirtyRect(const FloatRect &local_rect, const IntRect &transformed_clip_bounds, IntRect *dirty_rect);

    void WillOverwriteCanvas();

    bool shouldDrawShadows() const;

    void applyShadow(ShadowMode);

    void InflateStrokeRect(FloatRect &rect) const;

    void DrawPathInternal(Path &path,
                          CanvasRenderingContext2DState::PaintType paint_type,
                          SkPath::FillType = SkPath::kWinding_FillType);

    void DrawTextInternal(const base::String &text, float x, float y,
                          CanvasRenderingContext2DState::PaintType paint_type, float *max_width = nullptr);

    void DrawImageInternal(GraphicsContextSkia *c, Image *image, const FloatRect &src_rect,
                           const FloatRect &dst_rect, CompositeOperator compositeOperator,
                           BlendMode blendMode);

    void PutByteArray(const void *source, const IntSize &source_size, const IntRect &source_rect,
                      const IntPoint &dest_point);

    bool WritePixels(const SkImageInfo &orig_info, const void *pixels, size_t row_bytes, int x, int y);

    bool RectContainsTransformedRect(const FloatRect &rect, const IntRect &transformed_rect) const;

    void SkipQueuedDrawCommands();

    void StartRecording() const;

    bool ShouldDrawImageAntialiased(const FloatRect &dest_rect) const;

    void RestoreMatrixClipStack(SkCanvas *c) const;

    ResourceProvider *GetOrCreateResourceProvider(AccelerationHint hint = AccelerationHint::kPreferAcceleration);

    void FlushRecording();

    void DisableAcceleration();

    FloatPoint textOffset(float width, const FontMetrics &fontMetrics, TextDirection direction) const;
};

}


#endif //ATOMGRAPHICS_CANVASRENDERINGCONTEXT2DSKIA_H
