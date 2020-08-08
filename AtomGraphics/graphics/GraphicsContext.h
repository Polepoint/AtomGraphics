//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICSCONTEXT_H
#define ATOMGRAPHICSCONTEXT_H

/**
 * GraphicsContext
 * 简单而言，iOS上和CoreGraphics的Context的APIs差不多
 * Android上和SkCanvas的APIs差不多
 */

#include "base/ATString.h"
#include "node/canvas/CanvasStyle.h"
#include "node/canvas/CanvasTypes.h"
#include "node/canvas/CanvasRenderingContext2DState.h"
#include "Path.h"
#include "AffineTransform.h"
#include "Image.h"
#include "DashArray.h"

namespace AtomGraphics {

class GraphicsContext {

public:

    virtual ~GraphicsContext() {}

    virtual void setFillStyle(const CanvasStyle &) = 0;

    virtual void setStrokeStyle(const CanvasStyle &) = 0;

    virtual void setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor) = 0;

    virtual void setLineCap(const LineCap lineCap) = 0;

    virtual void setLineJoin(const LineJoin lineJoin) = 0;

    virtual void setStrokeThickness(float thickness) = 0;

    virtual void setMiterLimit(float limit) = 0;

    virtual void setLineDash(const DashArray &dash, float dashOffset) = 0;

    virtual void fillRect(const FloatRect &rect) = 0;

    virtual void strokeRect(const FloatRect &rect, float lineWidth) = 0;

    virtual void clearRect(const FloatRect &rect) = 0;

    virtual void fillPath(const Path &path) = 0;

    virtual void strokePath(Path *path) = 0;

    virtual void clipPath(Path *path, WindRule = WindRule::EvenOdd) = 0;

    virtual void clipRect(const FloatRect &rect) = 0;

    virtual void scale(float sx, float sy) = 0;

    virtual void rotate(float angleInRadians) = 0;

    virtual void translate(float x, float y) = 0;

    virtual void concat(const AffineTransform &ctm) = 0;

    virtual AffineTransform getTransform() = 0;

    virtual void setTransform(const AffineTransform &transform) = 0;

    virtual void drawImage(Image *image, const FloatRect &destRect, const FloatRect &srcRect,
                           CompositeOperator compositeOperator, BlendMode blendMode) = 0;

    virtual void drawText(const base::String &text, float x, float y, const Font &font, bool fill) = 0;

    virtual void setGlobalAlpha(float alpha) = 0;

    virtual void setCompositeOperation(CompositeOperator, BlendMode = BlendMode::Normal) = 0;

    virtual void save() = 0;

    virtual void restore() = 0;

    /**
     * triggers execution of all pending draw operations
     */
    virtual void flush() = 0;

    virtual void setImageInterpolationQuality(InterpolationQuality quality) = 0;

protected:

    FloatRect m_viewRect{FloatRect::ZERO};

};

}

#endif //ATOMGRAPHICSCONTEXT_H
