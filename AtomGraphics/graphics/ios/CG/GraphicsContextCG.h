//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTEXTGC_H
#define ATOMGRAPHICS_GRAPHICSCONTEXTGC_H

#import "GraphicsContextState.h"
#import "graphics/GraphicsContext.h"
#import "math/FloatRect.h"
#import "graphics/Path.h"
#import <CoreGraphics/CGContext.h>

namespace AtomGraphics {

class ImageBuffer;

enum RoundingMode {
    RoundAllSides,
    RoundOriginAndDimensions
};

class GraphicsContextCG final : public GraphicsContext {

public:

    GraphicsContextCG(RetainPtr<CGContextRef> platformContext);

    ~GraphicsContextCG();

    CGContextRef getCGContextRef() {
        return m_context.get();
    }

    void setFillStyle(const CanvasStyle &style) override;

    void setStrokeStyle(const CanvasStyle &style) override;

    void setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor) override;

    void setLineCap(const LineCap lineCap) override;

    void setLineJoin(const LineJoin lineJoin) override;

    void setStrokeThickness(float thickness) override;

    void setLineDash(const DashArray &dash, float dashOffset) override;

    void setMiterLimit(float limit) override;

    void fillRect(const FloatRect &rect) override;

    void strokeRect(const AtomGraphics::FloatRect &rect, float lineWidth) override;

    void clearRect(const FloatRect &rect) override;

    void fillPath(const AtomGraphics::Path &path) override;

    void strokePath(Path *path) override;

    void clipPath(Path *path, WindRule WindRule = WindRule::EvenOdd) override;

    void clipRect(const FloatRect &rect) override;

    void clipOut(const FloatRect &rect);

    void clipToImageBuffer(ImageBuffer &buffer, const FloatRect &destRect);

    void scale(float sx, float sy) override;

    void rotate(float angleInRadians) override;

    void translate(float x, float y) override;

    void concat(const AffineTransform &ctm) override;

    AffineTransform getTransform() override;

    void setTransform(const AffineTransform &transform) override;

    void drawImage(Image *image, const FloatRect &destRect, const FloatRect &srcRect, CompositeOperator compositeOperator, BlendMode blendMode = BlendMode::Normal) override;

    void drawText(const base::String &text, float x, float y, const Font &font, bool fill) override;

    void setGlobalAlpha(float alpha) override;

    void setCompositeOperation(CompositeOperator, BlendMode = BlendMode::Normal) override;

    void save() override;

    void restore() override;

    void flush() override;

    void beginCompositeLayer(float opacity = 1);

    void endCompositeLayer();

    void setImageInterpolationQuality(InterpolationQuality quality) override;

    FloatRect roundToDevicePixels(const FloatRect &, RoundingMode = RoundAllSides);

    WindRule fillRule() const { return m_state.fillRule; }

    void setFillRule(WindRule fillRule) {
        m_state.fillRule = fillRule;
    }

private:

    RetainPtr<CGContextRef> m_context;

    GraphicsContextState m_state;

    std::vector<GraphicsContextState> m_stack{std::vector<GraphicsContextState>(1)};

    bool hasVisibleShadow() const { return !m_state.shadowColor.equals(Color4F::TRANSPARENT); }

    bool hasShadow() const { return hasVisibleShadow() && (m_state.shadowBlur || m_state.shadowOffset.width || m_state.shadowOffset.height); }

    bool hasBlurredShadow() const { return hasVisibleShadow() && m_state.shadowBlur; }

    void applyStrokePattern();

    void applyFillPattern();

    bool paintingDisabled();

};

}


#endif //ATOMGRAPHICS_GRAPHICSCONTEXTGC_H
