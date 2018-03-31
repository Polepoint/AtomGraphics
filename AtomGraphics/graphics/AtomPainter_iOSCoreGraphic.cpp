//
// Created by neo on 2018/3/29.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomPainter_iOSCoreGraphic.h"

namespace atomgraphics {

    void Painter_iOSCoreGraphic::drawLine(const Vec2 &origin, const Vec2 &destination, const Color4F &color) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);
        CGContextBeginPath(platformContext);
        CGContextMoveToPoint(platformContext, origin.x, origin.y);
        CGContextAddLineToPoint(platformContext, destination.x, destination.y);
        CGContextStrokePath(platformContext);
        /*
         * antialias
         *  CGContextSetShouldAntialias(platformContext, true);
         */
        CGContextRestoreGState(platformContext);
    }

    void Painter_iOSCoreGraphic::strokeRect(float x, float y, float w, float h, Color4F color, float lineWidth) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);
        CGContextStrokeRectWithWidth(platformContext, CGRectMake(x, y, w, h), lineWidth);
        const CGFloat colorComponent[4] = {color.r, color.g, color.b, color.a};
        CGContextSetFillColor(platformContext, colorComponent);

        CGContextRestoreGState(platformContext);
    }

    void Painter_iOSCoreGraphic::fillRect(float x, float y, float w, float h, Color4F color) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);
        const CGFloat colorComponent[4] = {color.r, color.g, color.b, color.a};
        CGContextSetFillColor(platformContext, colorComponent);
        CGContextFillRect(platformContext, CGRectMake(x, y, w, h));

        CGContextRestoreGState(platformContext);
    }


    void Painter_iOSCoreGraphic::drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);


        CGContextRestoreGState(platformContext);
    }

    void Painter_iOSCoreGraphic::drawArc(const Vec2 &center, float radius, float startAngle, float endAngle, const Color4F &color) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);
        CGContextAddArc(platformContext, center.x, center.y, radius, startAngle, endAngle, 0);

        CGContextRestoreGState(platformContext);
    }

    void Painter_iOSCoreGraphic::drawText(const char *text, float x, float y, float maxWidth, bool isStroke) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);


        CGContextRestoreGState(platformContext);
    }

    void Painter_iOSCoreGraphic::drawImage(ImageProvider *imageProvider, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) {
        CGContextRef platformContext = this->getContext()->platformContext();
        CGContextSaveGState(platformContext);


        CGContextRestoreGState(platformContext);
    }

    Painter_iOSCoreGraphic::Painter_iOSCoreGraphic(GraphicsContext *context) : Painter(context) {

    }

}
