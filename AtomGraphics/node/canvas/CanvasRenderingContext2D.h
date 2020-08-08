//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASCONTEXT_H
#define ATOMCANVASCONTEXT_H

#include "CanvasRenderingContextBase.h"
#include "CanvasNode.h"
#include "CanvasTypes.h"
#include "CanvasStyle.h"
#include "base/AtomTypes.h"
#include "base/ATString.h"
#include "graphics/font/FontMetrics.h"
#include "graphics/font/TextMetrics.h"
#include "graphics/Pattern.h"
#include "graphics/ImageData.h"
#include "graphics/Gradient.h"
#include "graphics/GraphicsTypes.h"
#include "CanvasImageSource.h"
#include <string>

/**
 * CanvasAPI
 * https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D
 */

namespace AtomGraphics {

class CanvasNode;

class CanvasRenderingContext2D : public CanvasRenderingContextBase {

public:

    CanvasRenderingContext2D(CanvasNode *node) : m_node(node) {}

    virtual ~CanvasRenderingContext2D() = default;

    //color|gradient|pattern;
    virtual void setFillStyle(const CanvasStyle &canvasStyle);


    //color|gradient|pattern;
    virtual void setStrokeStyle(const CanvasStyle &canvasStyle);

    //color
    virtual void setShadowColor(const Color4F &color);

    //number
    virtual void setShadowBlur(float blur);

    //number
    virtual void setShadowOffsetX(float offsetX);

    //number
    virtual void setShadowOffsetY(float offsetY);

    //e.g. ctx.lineCap="round";
    //context.lineCap="butt|round|square";
    virtual void setLineCap(const LineCap cap) = 0;

    //e.g. ctx.lineJoin="round";
    //context.lineJoin="bevel|round|miter";
    virtual void setLineJoin(const LineJoin lineJoin) = 0;

    virtual void setLineDash(const std::vector<float> &dashArray) = 0;

    virtual void setLineDashOffset(float dashOffset) = 0;

    //e.g. ctx.lineWidth=10;
    //context.lineWidth=number;
    virtual void setLineWidth(float width);

    //e.g. ctx.miterLimit=5;
    //context.miterLimit=number;
    virtual void setMiterLimit(float limit);

    //e.g. ctx.rect(20,20,150,100);
    //context.rect(x,y,width,height);
    virtual void rect(float x, float y, float width, float height);

    //e.g. ctx.fillRect(20,20,150,100);
    //context.fillRect(x,y,width,height);
    virtual void fillRect(float x, float y, float width, float height);

    //e.g. ctx.strokeRect(20,20,150,100);
    //context.strokeRect(x,y,width,height);
    virtual void strokeRect(float x, float y, float width, float height);

    //e.g. ctx.clearRect(20,20,100,50);
    //context.clearRect(x,y,width,height);
    virtual void clearRect(float x, float y, float width, float height);

    //e.g. ctx.fill();
    //context.fill('nonzero' | 'evenodd'); default 'nonzero';
    virtual void fill(WindRule = WindRule::NonZero) = 0;

    //e.g. ctx.stroke();
    //context.stroke();
    virtual void stroke();

    //e.g. ctx.beginPath();
    //context.beginPath();
    virtual void beginPath();

    //e.g. ctx.moveTo(0,0);
    //context.moveTo(x,y);
    virtual void moveTo(float x, float y);

    //e.g. ctx.closePath();
    //context.closePath();
    virtual void closePath();

    //e.g. ctx.lineTo(300,150);
    //context.lineTo(x,y);
    virtual void lineTo(float x, float y);

    //e.g. ctx.clip();
    //context.clip();
    virtual void clip(WindRule = WindRule::NonZero) = 0;

    //e.g. ctx.quadraticCurveTo(20,100,200,20);
    //context.quadraticCurveTo(cpx,cpy,x,y);
    virtual void quadraticCurveTo(float cpx, float cpy, float x, float y);

    virtual void
    bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

    //e.g. ctx.arc(100,75,50,0,2*Math.PI);
    //context.arc(x,y,r,sAngle,eAngle,counterclockwise);
    virtual void
    arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise);

    //e.g. ctx.arcTo(150,20,150,70,50);
    //context.arcTo(x1,y1,x2,y2,r);
    virtual void arcTo(float x1, float y1, float x2, float y2, float r);

    //e.g. ctx.isPointInPath(20,50)
    //context.isPointInPath(x,y);
    virtual bool isPointInPath(float x, float y);

    //e.g. ctx.scale(2,2);
    //context.scale(scalewidth,scaleheight);
    virtual void scale(float sx, float sy);

    //e.g. ctx.rotate(20*Math.PI/180);
    //context.rotate(angle);
    virtual void rotate(float angleInRadians);

    //e.g. ctx.translate(70,70);
    //context.translate(x,y);
    virtual void translate(float tx, float ty);

    //e.g. ctx.transform(1,0.5,-0.5,1,30,10);
    //context.transform(a,b,c,d,e,f);
    virtual void transform(float a, float b, float c, float d, float tx, float ty);

    //e.g. ctx.setTransform(1,0.5,-0.5,1,30,10);
    //context.setTransform(a,b,c,d,e,f);
    virtual void setTransform(float a, float b, float c, float d, float tx, float ty);


    //e.g. ctx.font="40px Arial";
    virtual void setFont(const std::string &fontString) = 0;

    virtual std::string getFont() = 0;

    //e.g. ctx.textAlign="right";
    //context.textAlign="center|end|left|right|start";
    virtual void setTextAlign(TextAlign textAlign);

    //e.g. ctx.textBaseline="top";
    //context.textBaseline="alphabetic|top|hanging|middle|ideographic|bottom";
    virtual void setTextBaseline(TextBaseline textBaseline);

    //e.g. ctx.fillText("Hello World!",10,50);
    //context.fillText(text,x,y,maxWidth);
    virtual void fillText(const base::String &text, float x, float y, float *maxWidth);

    //e.g. ctx.strokeText("Hello World!",10,50);
    //context.strokeText(text,x,y,maxWidth);
    virtual void strokeText(const base::String &text, float x, float y, float *maxWidth);

    //e.g. ctx.measureText(txt).width
    //context.measureText(text);
    virtual TextMetrics measureText(const base::String &text) = 0;

    //e.g. ctx.drawImage(img,10,10);
    //context.drawImage(img,x,y);
    virtual void drawImage(CanvasImageSource *source, float x, float y);

    //e.g. ctx.drawImage(img,10,10,240,160);
    //context.drawImage(img,x,y,width,height);
    virtual void drawImage(CanvasImageSource *source, float x, float y, float width, float height);

    //e.g. ctx.drawImage(img,90,130,90,80,20,20,90,80);
    //context.drawImage(img,sx,sy,swidth,sheight,x,y,width,height);
    virtual void drawImage(CanvasImageSource *source,
                           float srcX, float srcY, float srcWidth, float srcHeight,
                           float dstX, float dstY, float dstWidth, float dstHeight) = 0;

    virtual scoped_refptr<ImageData> getImageData(int x, int y, int width, int height) = 0;

    virtual void putImageData(ImageData *imageData, int x, int y);

    virtual void putImageData(ImageData *imageData, int x, int y,
                              int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight) = 0;

    //e.g.  context.globalAlpha=number;
    virtual void setGlobalAlpha(float alpha) = 0;

    //e.g. ctx.globalCompositeOperation="source-over";
    virtual void setGlobalCompositeOperation(const CompositeOperator operation, const BlendMode blendMode) = 0;

    virtual void save() = 0;

    virtual void restore() = 0;

    virtual void createEvent();

    virtual void *toDataURL();

    virtual void enableConsumingDrawCache() = 0;

    virtual void releaseConsumingImage() = 0;

    virtual void paintInContext(GraphicsContext *context, const FloatRect &contentBounds) = 0;

    bool is2D() override;

    const FloatSize &contentSize() const {
        return m_node->contentSize();
    }

    virtual void reset() = 0;

protected:

    CanvasNode *m_node;
};
}


#endif //ATOMGRAPHICS_ATOMCANVASCONTEXT_H
