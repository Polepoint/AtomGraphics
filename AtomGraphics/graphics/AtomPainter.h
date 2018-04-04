//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMPAINTER_H
#define ATOMPAINTER_H

#include "AtomGraphicsContext.h"
#include "AtomImageProvider.h"
#include "AtomTypes.h"

namespace AtomGraphics {

    class GraphicsContext;

    class Painter {

    public:

        virtual GraphicsContext *getContext() final;

        // draw line
        virtual void drawLine(const Vec2 &origin, const Vec2 &destination, const Color4F &color);

        // draw rectangle
        virtual void strokeRect(float x, float y, float w, float h, Color4F color, float lineWidth);

        virtual void fillRect(float x, float y, float w, float h, Color4F color);

        // Draws a polygon
        virtual void drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color);

        // draw circle
        virtual void drawArc(const Vec2 &center, float radius, float startAngle, float endAngle, const Color4F &color);

        // draw text
        virtual void drawText(const char *text, float x, float y, float maxWidth, bool isStroke);

        // draw image
        virtual void drawImage(ImageProvider *imageProvider, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);


        Painter(GraphicsContext *context);

        ~Painter();

    private:

        GraphicsContext *_context;
    };

}

#endif //ATOMPAINTER_H
