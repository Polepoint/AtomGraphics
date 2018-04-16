//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASCONTEXT_H
#define ATOMCANVASCONTEXT_H

#include "AtomImageBuffer.h"
#include "AtomCanvasGradient.h"
#include "AtomCanvasPattern.h"
#include "AtomCanvasContextBase.h"

/**
 * CanvasAPI
 * http://www.w3school.com.cn/tags/html_ref_canvas.asp
 */

namespace AtomGraphics {

    class CanvasContext2d :public CanvasContextBase{

    public:
//        using Style = Variant<std::string, CanvasGradient, CanvasPattern>;
        //color|gradient|pattern;
        virtual void setFillStyle(const Color4F &color);

        virtual void setFillStyle(const CanvasPattern &pattern);

        virtual void setFillStyle(const CanvasGradient &gradient);

        //color|gradient|pattern;
        virtual void setStrokeStyle(const Color4F &color);

        virtual void setStrokeStyle(const CanvasPattern &pattern);

        virtual void setStrokeStyle(const CanvasGradient &gradient);

        //color
        virtual void setShadowColor();

        //number
        virtual void setShadowBlur();

        //number
        virtual void setShadowOffsetX();

        //number
        virtual void setShadowOffsetY();

        //context.createPattern(image,"repeat|repeat-x|repeat-y|no-repeat");
        virtual CanvasPattern *createPattern();

        //context.createLinearGradient(x0,y0,x1,y1);
        //return object that represent the gradient
        virtual CanvasGradient *createLinearGradient(float x0, float y0, float x1, float y1);

        //context.createRadialGradient(x0,y0,r0,x1,y1,r1);
        virtual void *createRadialGradient();

        //var grd=ctx.createLinearGradient(0,0,170,0);
        //grd.addColorStop(0,"black");


        //eg. ctx.lineCap="round";
        //context.lineCap="butt|round|square";
        virtual void setLineCap(const std::string lineCap);

        //eg. ctx.lineJoin="round";
        //context.lineJoin="bevel|round|miter";
        virtual void setLineJoin(const std::string lineJoin);

        //eg. ctx.lineWidth=10;
        //context.lineWidth=number;
        virtual void setLineWidth(float width);

        //eg. ctx.miterLimit=5;
        //context.miterLimit=number;
        virtual void setMiterLimit(float limit);

        //eg. ctx.rect(20,20,150,100);
        //context.rect(x,y,width,height);
        virtual void setRect(float x, float y, float width, float height);

        //eg. ctx.fillRect(20,20,150,100);
        //context.fillRect(x,y,width,height);
        virtual void fillRect(float x, float y, float width, float height);

        //eg. ctx.strokeRect(20,20,150,100);
        //context.strokeRect(x,y,width,height);
        virtual void strokeRect(float x, float y, float width, float height);

        //eg. ctx.clearRect(20,20,100,50);
        //context.clearRect(x,y,width,height);
        virtual void clearRect(float x, float y, float width, float height);

        //eg. ctx.fill();
        //context.fill('nonzero' | 'evenodd'); default 'nonzero';
        virtual void fill();

        //eg. ctx.stroke();
        //context.stroke();
        virtual void stroke();

        //eg. ctx.beginPath();
        //context.beginPath();
        virtual void beginPath();

        //eg. ctx.moveTo(0,0);
        //context.moveTo(x,y);
        virtual void moveTo(float x, float y);

        //eg. ctx.closePath();
        //context.closePath();
        virtual void closePath();

        //eg. ctx.lineTo(300,150);
        //context.lineTo(x,y);
        virtual void lineTo(float x, float y);

        //eg. ctx.clip();
        //context.clip();
        virtual void clip();

        //eg. ctx.quadraticCurveTo(20,100,200,20);
        //context.quadraticCurveTo(cpx,cpy,x,y);
        virtual void quadraticCurveTo(float cpx, float cpy, float x, float y);

        virtual void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

        //eg. ctx.arc(100,75,50,0,2*Math.PI);
        //context.arc(x,y,r,sAngle,eAngle,counterclockwise);
        virtual void arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise);

        //eg. ctx.arcTo(150,20,150,70,50);
        //context.arcTo(x1,y1,x2,y2,r);
        virtual void arcTo(float x1, float y1, float x2, float y2, float r);

        //eg. ctx.isPointInPath(20,50)
        //context.isPointInPath(x,y);
        virtual void isPointInPath(float x, float y);

        //eg. ctx.scale(2,2);
        //context.scale(scalewidth,scaleheight);
        virtual void scale(float scaleWidth, float scaleHeight);

        //eg. ctx.rotate(20*Math.PI/180);
        //context.rotate(angle);
        virtual void rotate(double angle);

        //eg. ctx.translate(70,70);
        //context.translate(x,y);
        virtual void translate(float x, float y);

        //eg. ctx.transform(1,0.5,-0.5,1,30,10);
        //context.transform(a,b,c,d,e,f);
        virtual void transform(float a, float b, float c, float d, float e, float f);

        //eg. ctx.setTransform(1,0.5,-0.5,1,30,10);
        //context.setTransform(a,b,c,d,e,f);
        virtual void setTransform(float a, float b, float c, float d, float e, float f);

        //eg. ctx.font="40px Arial";
        virtual void setFont(const std::string font);

        //eg. ctx.textAlign="right";
        //context.textAlign="center|end|left|right|start";
        virtual void setTextAlign(const std::string textAlign);

        //eg. ctx.textBaseline="top";
        //context.textBaseline="alphabetic|top|hanging|middle|ideographic|bottom";
        virtual void setTextBaseline(const std::string textBaseline);

        //eg. ctx.fillText("Hello World!",10,50);
        //context.fillText(text,x,y,maxWidth);
        virtual void fillText(std::string text, float x, float y, float maxWidth);

        //eg. ctx.strokeText("Hello World!",10,50);
        //context.strokeText(text,x,y,maxWidth);
        virtual void strokeText(const std::string text, float x, float y, float maxWidth);

        //eg. ctx.measureText(txt).width
        //context.measureText(text);
        virtual void *measureText(const std::string &text);

        //eg. ctx.drawImage(img,10,10);
        //context.drawImage(img,x,y);
        virtual void *drawImage(ImageBuffer *imageBuffer, float x, float y);

        //eg. ctx.drawImage(img,10,10,240,160);
        //context.drawImage(img,x,y,width,height);
        virtual void *drawImage(ImageBuffer *imageBuffer, float x, float y, float with, float height);

        //eg. ctx.drawImage(img,90,130,90,80,20,20,90,80);
        //context.drawImage(img,sx,sy,swidth,sheight,x,y,width,height);
        virtual void *drawImage(ImageBuffer *imageBuffer, float sx, float sy, float swith, float sheight, float x, float y, float width, float height);

        //eg. var imgData=ctx.createImageData(100,100);
        //var imgData=context.createImageData(width,height);
        virtual void *createImageData(float width, float height);

        //eg. var imgData=context.createImageData(imageData);
        //var imgData=context.createImageData(imageData);
        virtual void *createImageData(void *imageData);


        //eg.  context.globalAlpha=number;
        virtual void setGlobalAlpha(float number);

        //eg. ctx.globalCompositeOperation="source-over";
        virtual void setGlobalCompositeOperation(const std::string operation);

        virtual void save();

        virtual void restore();

        virtual void createEvent();

        virtual void *getContext();

        virtual void *toDataURL();

        bool is2d() override;

        virtual void drawConsuming(const GraphicsContext *context, Rect destRect);

        virtual void ensureDrawingContext();

        void setContentSize(const Size &contentSize);

    protected:

        PlatformContext _drawingContext;
        PlatformPath _path;
        uint8_t *_imageData;
        Size _contentSize;

        virtual PlatformPath ensurePlatformPath();
    };
}


#endif //ATOMGRAPHICS_ATOMCANVASCONTEXT_H
