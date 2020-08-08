//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASCONTEXT2DCG_H
#define ATOMCANVASCONTEXT2DCG_H
/**
 * CG: CoreGraphics
 */

#import "CanvasRenderingContext2D.h"
#import "CanvasNodeCG.h"
#import "GraphicsContentFlushController.h"
#import "CanvasStyle.h"
#import "AffineTransform.h"
#import "Font.h"
#import "CanvasTypes.h"
#import "ATString.h"
#import "TextMetrics.h"
#import "CanvasRenderingContext2DState.h"
#import "Path.h"
#import "CanvasImageSource.h"
#import "graphics/ios/CG/GraphicsContextCG.h"
#import <CoreGraphics/CGBitmapContext.h>
#import <stack>

namespace AtomGraphics {

enum CanvasDidDrawOption {
    CanvasDidDrawApplyNone = 0,
    CanvasDidDrawApplyTransform = 1,
    CanvasDidDrawApplyShadow = 1 << 1,
    CanvasDidDrawApplyClip = 1 << 2,
    CanvasDidDrawApplyAll = 0xffffffff
};


class CanvasRenderingContext2DCG final : public CanvasRenderingContext2D {

public:

    CanvasRenderingContext2DCG(CanvasNode *node);

    ~CanvasRenderingContext2DCG() override;

    void setFillStyle(const CanvasStyle &canvasStyle) override;

    void setStrokeStyle(const CanvasStyle &canvasStyle) override;

    void setShadowColor(const Color4F &color) override;

    void setShadowBlur(float blur) override;

    void setShadowOffsetX(float offsetX) override;

    void setShadowOffsetY(float offsetY) override;

    void setLineCap(const LineCap lineCap) override;

    void setLineJoin(const LineJoin lineJoin) override;

    void setLineWidth(float width) override;

    void setLineDash(const std::vector<float> &dashArray) override;

    void setLineDashOffset(float dashOffset) override;

    void setMiterLimit(float limit) override;

    void rect(float x, float y, float width, float height) override;

    void fillRect(float x, float y, float width, float height) override;

    void strokeRect(float x, float y, float width, float height) override;

    void clearRect(float x, float y, float width, float height) override;

    void fill(WindRule fillRule) override;

    void stroke() override;

    void inflateStrokeRect(FloatRect &rect);

    void beginPath() override;

    void moveTo(float x, float y) override;

    void closePath() override;

    void lineTo(float x, float y) override;

    void clip(WindRule = WindRule::NonZero) override;

    void quadraticCurveTo(float cpx, float cpy, float x, float y) override;

    void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) override;

    void arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise) override;

    void arcTo(float x1, float y1, float x2, float y2, float r) override;

    bool isPointInPath(float x, float y) override;

    void scale(float sx, float sy) override;

    void rotate(float angleInRadians) override;

    void translate(float x, float y) override;

    void transform(float a, float b, float c, float d, float tx, float ty) override;

    void setTransform(float a, float b, float c, float d, float tx, float ty) override;

    void resetTransform();

    void setFont(const std::string &font) override;

    std::string getFont() override;

    void setTextAlign(TextAlign textAlign) override;

    void setTextBaseline(TextBaseline textBaseline) override;

    void fillText(const base::String &text, float x, float y, float *maxWidth) override;

    void strokeText(const base::String &text, float x, float y, float *maxWidth) override;

    TextMetrics measureText(const base::String &text) override;

    void drawImage(CanvasImageSource *source, float srcX, float srcY, float srcWidth, float srcHeight, float dstX, float dstY, float dstWidth, float dstHeight) override;

    scoped_refptr<ImageData> getImageData(int x, int y, int width, int height) override;

    void putImageData(ImageData *imageData, int x, int y, int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight) override;

    void setGlobalAlpha(float number) override;

    void setGlobalCompositeOperation(const CompositeOperator operation, const BlendMode blendMode) override;

    void save() override;

    void restore() override;

    void createEvent() override;

    void *toDataURL() override;

    void enableConsumingDrawCache() override;

    void releaseConsumingImage() override;

    void paintInContext(GraphicsContext *context, const FloatRect &contentBounds) override;

    ImageBuffer *buffer() const {
        return m_imageBuffer.get();
    }

    void reset() override;

private:

    std::map<IntSize, std::string> m_md5Data;

    int m_unrealizedSaveCount{0};

    scoped_refptr<Image> m_consumingDrawCachedImage{nullptr};
    bool m_cacheConsuming{false};

    scoped_refptr<ImageBuffer> m_imageBuffer;
    std::unique_ptr<GraphicsContextCG> m_drawingContext{nullptr};
    std::unique_ptr<Path> m_path{nullptr};
    std::vector<CanvasRenderingContext2DState> m_stateStack;

    FloatRect m_dirtyRect{FloatRect::ZERO};

    GraphicsContextCG *drawingContext();

    void updateDrawingContext();

    void ensurePlatformPath();

    CanvasRenderingContext2DState &currentState();

    ImageBuffer *createCompositingBuffer(const IntRect &bufferRect);

    Path transformAreaToDevice(const Path &path);

    Path transformAreaToDevice(const FloatRect &rect);

    IntRect calculateCompositingBufferRect(const FloatRect &area, IntSize *croppedOffset);

    void fullCanvasCompositedDrawImage(Image &image, const FloatRect &dest, const FloatRect &src, CompositeOperator op);

    void compositeBuffer(ImageBuffer &buffer, const IntRect &bufferRect, CompositeOperator op);

    void applyShadow();

    void applyShadow(const FloatSize &offset, const float blur, const Color4F &shadowColor);

    bool shouldDrawShadows();

    void applyLineDash();

    bool rectContainsCanvas(const FloatRect &);

    FloatPoint textOffset(float width, TextDirection direction);

    void drawText(const base::String &text, float x, float y, bool fill, float *maxWidth);

    FloatPoint textOffset(float width);

    void realizeSaves();

    void realizeSavesLoop();

    bool isFullCanvasCompositeMode(CompositeOperator op);

    void clearCanvas();

    void didDrawEntireCanvas();

    void didDraw(const FloatRect &r, unsigned options = CanvasDidDrawApplyAll);

    friend class CanvasNodeCG;
};

}

#endif //ATOMCANVASCONTEXT2DCG_H
