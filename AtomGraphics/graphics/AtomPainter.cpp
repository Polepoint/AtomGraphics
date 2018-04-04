//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomPainter.h"

namespace AtomGraphics {

    GraphicsContext *Painter::getContext() {
        return _context;
    }

    void Painter::drawLine(const Vec2 &origin, const Vec2 &destination, const Color4F &color) {

    }

    void Painter::strokeRect(float x, float y, float w, float h, Color4F color, float lineWidth) {

    }

    void Painter::fillRect(float x, float y, float w, float h, Color4F color) {

    }

    void Painter::drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color) {

    }


    void Painter::drawArc(const Vec2 &center, float radius, float startAngle, float endAngle, const Color4F &color) {

    }

    void Painter::drawText(const char *text, float x, float y, float maxWidth, bool isStroke) {

    }

    void Painter::drawImage(ImageProvider *imageProvider, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) {

    }

    Painter::Painter(GraphicsContext *context) {
        _context = context;
    }

    Painter::~Painter() {

    }

}



