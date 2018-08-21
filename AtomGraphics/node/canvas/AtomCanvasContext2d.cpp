//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasContext2D.h"

namespace AtomGraphics {

    void CanvasContext2D::setFillStyle(const Color4F &color) {

    }

    void CanvasContext2D::setFillStyle(const AtomCanvasPattern *pattern) {

    }

    void CanvasContext2D::setFillStyle(const AtomGraphics::AtomCanvasLinearGradient *gradient) {

    }

    void CanvasContext2D::setFillStyle(const AtomGraphics::AtomCanvasRadialGradient *gradient) {

    }

    void CanvasContext2D::setStrokeStyle(const Color4F &color) {

    }

    void CanvasContext2D::setStrokeStyle(const CanvasPattern &pattern) {

    }

    void CanvasContext2D::setStrokeStyle(const CanvasGradient &gradient) {

    }

    void CanvasContext2D::setShadowColor(const Color4F &color) {

    }

    void CanvasContext2D::setShadowBlur(const int shadowBlur) {

    }

    void CanvasContext2D::setShadowOffsetX(const float offsetX) {

    }

    void CanvasContext2D::setShadowOffsetY(const float offsetY) {

    }

    AtomCanvasPattern *CanvasContext2D::createPattern(const std::string &style, const AtomCanvasImage *image) {
        return nullptr;
    }

    AtomCanvasLinearGradient *CanvasContext2D::createLinearGradient(float x0, float y0, float x1, float y1) {
        return nullptr;
    }

    AtomCanvasRadialGradient *CanvasContext2D::createRadialGradient(float x0, float y0, float r0, float x1, float y1, float r1) {
        return nullptr;
    }

    void CanvasContext2D::setLineCap(const std::string lineCap) {

    }

    void CanvasContext2D::setLineJoin(const std::string lineJoin) {

    }

    void CanvasContext2D::setLineWidth(double width) {

    }

    void CanvasContext2D::setMiterLimit(float limit) {

    }

    void CanvasContext2D::setRect(float x, float y, float width, float height) {

    }

    void CanvasContext2D::fillRect(float x, float y, float width, float height) {

    }

    void CanvasContext2D::strokeRect(float x, float y, float width, float height) {

    }

    void CanvasContext2D::clearRect(float x, float y, float width, float height) {

    }

    void CanvasContext2D::fill() {
    }

    void CanvasContext2D::stroke() {
    }

    void CanvasContext2D::beginPath() {
    }

    void CanvasContext2D::moveTo(double x, double y) {

    }

    void CanvasContext2D::closePath() {

    }

    void CanvasContext2D::lineTo(double x, double y) {

    }

    void CanvasContext2D::clip() {

    }

    void CanvasContext2D::quadraticCurveTo(float cpx, float cpy, float x, float y) {

    }

    void CanvasContext2D::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {

    }

    void CanvasContext2D::arc(double x, double y, double r, double sAngle, double eAngle, bool counterclockwise) {

    }

    void CanvasContext2D::arcTo(float x1, float y1, float x2, float y2, float r) {

    }

    void CanvasContext2D::isPointInPath(float x, float y) {

    }

    void CanvasContext2D::scale(float scaleWidth, float scaleHeight) {

    }

    void CanvasContext2D::rotate(double angle) {

    }

    void CanvasContext2D::translate(float x, float y) {

    }

    void CanvasContext2D::transform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2D::setTransform(double a, double b, double c, double d, double e, double f) {

    }

    void CanvasContext2D::setFont(const std::string font) {

    }

    void CanvasContext2D::setTextAlign(const std::string textAlign) {

    }

    void CanvasContext2D::setTextBaseline(const std::string textBaseline) {

    }

    void CanvasContext2D::fillText(std::string text, double x, double y, float maxWidth) {

    }

    void CanvasContext2D::strokeText(const std::string text, double x, double y, float maxWidth) {

    }

    float CanvasContext2D::measureText(const std::string &text) {
        return 0;
    }

    void CanvasContext2D::drawImage(AtomCanvasImage *image, float x, float y) {
    }

    void CanvasContext2D::drawImage(AtomCanvasImage *image, float x, float y, float width, float height) {
    }

    void CanvasContext2D::drawImage(AtomCanvasImage *image, float sx, float sy, float swidth, float sheight, float x, float y, float width, float height) {
    }

    void CanvasContext2D::drawImage(CanvasNode *canvasNode) {

    }

    AtomCanvasImageData *CanvasContext2D::createImageData(int width, int height) {
        return nullptr;
    }

    AtomCanvasImageData *CanvasContext2D::createImageData(AtomCanvasImageData *imageData) {
        return nullptr;
    }

    void *CanvasContext2D::getImageData(int x, int y, int width, int height) {
        return nullptr;
    }

    void CanvasContext2D::putImageData(AtomCanvasImageData *imageData, int x, int y, int srcWidth, int srcHeight, int destWidth, int destHeight) {

    }

    void CanvasContext2D::setGlobalAlpha(double number) {

    }

    void CanvasContext2D::setGlobalCompositeOperation(const std::string operation) {

    }

    void CanvasContext2D::save() {

    }

    void CanvasContext2D::restore() {

    }

    void CanvasContext2D::createEvent() {

    }

    void *CanvasContext2D::getContext() {
        return nullptr;
    }

    void *CanvasContext2D::toDataURL() {
        return nullptr;
    }

    void CanvasContext2D::setContentSize(const Size &contentSize) {
//        _contentSize = contentSize;
    }

    void CanvasContext2D::drawConsuming(const GraphicsContext *context, Rect destRect) {

    }

    void CanvasContext2D::ensureDrawingContext() {

    }

    bool CanvasContext2D::is2D() {
        return true;
    }

}