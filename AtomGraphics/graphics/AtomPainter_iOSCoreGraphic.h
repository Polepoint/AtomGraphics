//
// Created by neo on 2018/3/29.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMPAINTERIOSCOREGRAPHIC_H
#define ATOMPAINTERIOSCOREGRAPHIC_H


#include "AtomPainter.h"

class Painter_iOSCoreGraphic: public Painter {

    void drawLine(GraphicsContext *context, const Vec2 &origin, const Vec2 &destination, const Color4F &color) override;

    void strokeRect(GraphicsContext *context, float x, float y, float w, float h, Color4F color, float lineWidth) override;

    void fillRect(GraphicsContext *context, float x, float y, float w, float h, Color4F color) override;

    void drawPoly(GraphicsContext *context, const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color) override;

    void drawArc(GraphicsContext *context, const Vec2 &center, float radius, float startAngle, float endAngle, const Color4F &color) override;

    void drawText(GraphicsContext *context, const char *text, float x, float y, float maxWidth, bool isStroke) override;

    void drawImage(GraphicsContext *context, ImageProvider *imageProvider, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) override;

};


#endif //ATOMPAINTERIOSCOREGRAPHIC_H
