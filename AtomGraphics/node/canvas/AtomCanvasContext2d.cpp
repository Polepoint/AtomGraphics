//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasContext2d.h"

namespace AtomGraphics {

    void CanvasContext2d::setFillStyle(const Color4F &color) {

    }

    void CanvasContext2d::setFillStyle(const CanvasPattern &pattern) {

    }

    void CanvasContext2d::setFillStyle(const CanvasGradient &gradient) {

    }

    void CanvasContext2d::setStrokeStyle(const Color4F &color) {

    }

    void CanvasContext2d::setStrokeStyle(const CanvasPattern &pattern) {

    }

    void CanvasContext2d::setStrokeStyle(const CanvasGradient &gradient) {

    }

    void CanvasContext2d::setShadowColor() {

    }

    void CanvasContext2d::setShadowBlur() {

    }

    void CanvasContext2d::setShadowOffsetX() {

    }

    void CanvasContext2d::setShadowOffsetY() {

    }

    CanvasPattern *CanvasContext2d::createPattern() {
        return nullptr;
    }

    CanvasGradient *CanvasContext2d::createLinearGradient(float x0, float y0, float x1, float y1) {
        return nullptr;
    }

    void *CanvasContext2d::createRadialGradient() {
        return nullptr;
    }

    void CanvasContext2d::setLineCap(const std::string lineCap) {

    }

    void CanvasContext2d::setLineJoin(const std::string lineJoin) {

    }

    void CanvasContext2d::setLineWidth(float width) {

    }

    void CanvasContext2d::setMiterLimit(float limit) {

    }

    void CanvasContext2d::setRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::fillRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::strokeRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::clearRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::fill() {
    }

    void CanvasContext2d::stroke() {
    }

    void CanvasContext2d::beginPath() {
    }

    void CanvasContext2d::moveTo(float x, float y) {

    }

    void CanvasContext2d::closePath() {
        CGContextClosePath(_drawingContext);
    }

    void CanvasContext2d::lineTo(float x, float y) {

    }

    void CanvasContext2d::clip() {

    }

    void CanvasContext2d::quadraticCurveTo(float cpx, float cpy, float x, float y) {

    }

    void CanvasContext2d::arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise) {

    }

    void CanvasContext2d::arcTo(float x1, float y1, float x2, float y2, float r) {

    }

    void CanvasContext2d::isPointInPath(float x, float y) {

    }

    void CanvasContext2d::scale(float scaleWidth, float scaleHeight) {

    }

    void CanvasContext2d::rotate(double angle) {

    }

    void CanvasContext2d::translate(float x, float y) {

    }

    void CanvasContext2d::transform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2d::setTransform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2d::setFont(const std::string font) {

    }

    void CanvasContext2d::setTextAlign() {

    }

    void CanvasContext2d::setTextBaseline() {

    }

    void CanvasContext2d::fillText(std::string text, float x, float y, float maxWidth) {

    }

    void CanvasContext2d::strokeText(const std::string &text, float x, float y, float maxWidth) {

    }

    void *CanvasContext2d::measureText(const std::string &text) {
        return nullptr;
    }

    void *CanvasContext2d::drawImage(ImageBuffer *imageBuffer, float x, float y) {
        return nullptr;
    }

    void *CanvasContext2d::drawImage(ImageBuffer *imageBuffer, float x, float y, float with, float height) {
        return nullptr;
    }

    void *CanvasContext2d::drawImage(ImageBuffer *imageBuffer, float sx, float sy, float swith, float sheight, float x, float y, float width, float height) {
        return nullptr;
    }

    void *CanvasContext2d::createImageData(float width, float height) {
        return nullptr;
    }

    void *CanvasContext2d::createImageData(void *imageData) {
        return nullptr;
    }

    void CanvasContext2d::setGlobalAlpha(float number) {

    }

    void CanvasContext2d::setGlobalCompositeOperation() {

    }

    void CanvasContext2d::save() {

    }

    void CanvasContext2d::restore() {

    }

    void CanvasContext2d::createEvent() {

    }

    void *CanvasContext2d::getContext() {
        return nullptr;
    }

    void *CanvasContext2d::toDataURL() {
        return nullptr;
    }

    void CanvasContext2d::setContentSize(const Size &contentSize) {
        _contentSize = contentSize;
    }

    void CanvasContext2d::drawConsuming(const GraphicsContext *context, Rect destRect) {

    }

    void CanvasContext2d::ensureDrawingContext() {

    }

    PlatformPath CanvasContext2d::ensurePlatformPath() {
        return nullptr;
    }

    bool CanvasContext2d::is2d() {
        return true;
    }

}