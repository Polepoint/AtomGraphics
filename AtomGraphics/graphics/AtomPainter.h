//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMPAINTER_H
#define ATOMPAINTER_H

#include "AtomGraphicsContext.h"
#include "AtomImageProvider.h"
#include "AtomTypes.h"

using namespace atomgraphics;

class Painter {

    // draw line
    virtual void drawLine(GraphicsContext *context, const Vec2 &origin, const Vec2 &destination, const Color4F &color);

    // draw rectangle
    virtual void strokeRect(GraphicsContext *context, float x, float y, float w, float h, Color4F color, float lineWidth);

    virtual void fillRect(GraphicsContext *context, float x, float y, float w, float h, Color4F color);

    // Draws a polygon
    virtual void drawPoly(GraphicsContext *context, const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color);

    // draw circle
    virtual void drawArc(GraphicsContext *context, const Vec2 &center, float radius, float startAngle, float endAngle, const Color4F &color);

    // draw text
    virtual void drawText(GraphicsContext *context, const char *text, float x, float y, float maxWidth, bool isStroke);

    // draw image
    virtual void drawImage(GraphicsContext *context, ImageProvider *imageProvider, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);
};


#endif //ATOMPAINTER_H
