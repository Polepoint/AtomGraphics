//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CanvasRenderingContext2D.h"
#include "CanvasTypes.h"
#include "graphics/font/FontMetrics.h"
#include "graphics/font/TextMetrics.h"

namespace AtomGraphics {

void CanvasRenderingContext2D::setFillStyle(const CanvasStyle &canvasStyle) {

}


void CanvasRenderingContext2D::setStrokeStyle(const CanvasStyle &canvasStyle) {

}

void CanvasRenderingContext2D::setShadowColor(const Color4F &color) {

}

void CanvasRenderingContext2D::setShadowBlur(float blur) {

}

void CanvasRenderingContext2D::setShadowOffsetX(float offsetX) {

}

void CanvasRenderingContext2D::setShadowOffsetY(float offsetY) {

}

void CanvasRenderingContext2D::setLineWidth(float width) {

}

void CanvasRenderingContext2D::setMiterLimit(float limit) {

}

void CanvasRenderingContext2D::rect(float x, float y, float width, float height) {

}

void CanvasRenderingContext2D::fillRect(float x, float y, float width, float height) {

}

void CanvasRenderingContext2D::strokeRect(float x, float y, float width, float height) {

}

void CanvasRenderingContext2D::clearRect(float x, float y, float width, float height) {

}

void CanvasRenderingContext2D::stroke() {
}

void CanvasRenderingContext2D::beginPath() {
}

void CanvasRenderingContext2D::moveTo(float x, float y) {

}

void CanvasRenderingContext2D::closePath() {

}

void CanvasRenderingContext2D::lineTo(float x, float y) {

}

void CanvasRenderingContext2D::quadraticCurveTo(float cpx, float cpy, float x, float y) {

}

void CanvasRenderingContext2D::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x,
                                             float y) {

}

void CanvasRenderingContext2D::arc(float x, float y, float r, float sAngle, float eAngle,
                                   bool counterclockwise) {

}

void CanvasRenderingContext2D::arcTo(float x1, float y1, float x2, float y2, float r) {

}

bool CanvasRenderingContext2D::isPointInPath(float x, float y) {
    return false;
}

void CanvasRenderingContext2D::scale(float sx, float sy) {

}

void CanvasRenderingContext2D::rotate(float angleInRadians) {

}

void CanvasRenderingContext2D::translate(float tx, float ty) {

}

void CanvasRenderingContext2D::transform(float a, float b, float c, float d, float tx, float ty) {

}

void CanvasRenderingContext2D::setTransform(float a, float b, float c, float d, float tx, float ty) {

}

void CanvasRenderingContext2D::setTextAlign(TextAlign textAlign) {

}

void CanvasRenderingContext2D::setTextBaseline(TextBaseline textBaseline) {

}

void CanvasRenderingContext2D::fillText(const base::String &text, float x, float y, float *maxWidth) {

}

void CanvasRenderingContext2D::strokeText(const base::String &text, float x, float y, float *maxWidth) {

}

void CanvasRenderingContext2D::drawImage(CanvasImageSource *source, float x, float y) {
    drawImage(source, x, y, source->size().width, source->size().height);
}

void CanvasRenderingContext2D::drawImage(CanvasImageSource *source, float x, float y, float width, float height) {
    drawImage(source, 0, 0, source->size().width, source->size().height, x, y, width, height);
}

void CanvasRenderingContext2D::putImageData(ImageData *imageData, int x, int y) {
    putImageData(imageData, x, y, 0, 0, imageData->size().width, imageData->size().height);
}

void CanvasRenderingContext2D::createEvent() {

}

void *CanvasRenderingContext2D::toDataURL() {
    return nullptr;
}

bool CanvasRenderingContext2D::is2D() {
    return true;
}

}