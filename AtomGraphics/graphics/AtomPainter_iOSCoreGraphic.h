//
// Created by neo on 2018/3/29.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMPAINTERIOSCOREGRAPHIC_H
#define ATOMPAINTERIOSCOREGRAPHIC_H


#include "AtomPainter.h"

namespace atomgraphics {

    class Painter_iOSCoreGraphic : public Painter {

    public:

        void drawLine(const Vec2 &origin, const Vec2 &destination, const Color4F &color) override;

        void strokeRect(float x, float y, float w, float h, Color4F color, float lineWidth) override;

        void fillRect(float x, float y, float w, float h, Color4F color) override;

        void drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color) override;

        void drawArc(const Vec2 &center, float radius, float startAngle, float endAngle, const Color4F &color) override;

        void drawText(const char *text, float x, float y, float maxWidth, bool isStroke) override;

        void drawImage(ImageProvider *imageProvider, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) override;

        Painter_iOSCoreGraphic(GraphicsContext *context);
    };

}

#endif //ATOMPAINTERIOSCOREGRAPHIC_H
