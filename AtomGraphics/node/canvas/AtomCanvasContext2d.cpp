//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasContext2d.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreGraphics/CGBitmapContext.h>

#endif

namespace AtomGraphics {

    void CanvasContext2d::setFillStyle(const Color4F &color) {
        const CGFloat colorComponents[4] = {color.r, color.g, color.b, color.a};
        // TODO: colorSpace should be cached
        CGContextSetFillColorWithColor(_drawingContext, CGColorCreate(CGColorSpaceCreateDeviceRGB(), colorComponents));
    }

    void CanvasContext2d::setFillStyle(const CanvasPattern &pattern) {

    }

    void CanvasContext2d::setFillStyle(const CanvasGradient &gradient) {

    }

    void CanvasContext2d::setStrokeStyle(const Color4F &color) {
        const CGFloat colorComponents[4] = {color.r, color.g, color.b, color.a};
        CGContextSetStrokeColor(_drawingContext, colorComponents);
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

    void CanvasContext2d::setLineCap() {

    }

    void CanvasContext2d::setLineJoin() {

    }

    void CanvasContext2d::setLineWidth() {

    }

    void CanvasContext2d::setMiterLimit() {

    }

    void CanvasContext2d::setRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::fillRect() {

    }

    void CanvasContext2d::strokeRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::clearRect(float x, float y, float width, float height) {

    }

    void CanvasContext2d::fill() {
//        CGContextFillPath(_drawingContext);
        CGContextDrawPath(_drawingContext, kCGPathFillStroke); //绘制路径加填充
    }

    void CanvasContext2d::stroke() {
//        CGContextStrokePath(_drawingContext);
        CGContextDrawPath(_drawingContext, kCGPathStroke); //绘制路径加填充
    }

    void CanvasContext2d::beginPath() {
        CGContextBeginPath(_drawingContext);
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

    void CanvasContext2d::arc(float x, float y, float r, float sAngle, float eAngle, int counterclockwise) {
        CGContextAddArc(_drawingContext, x, y, r, sAngle, eAngle, counterclockwise);
    }

    void CanvasContext2d::arcTo(float x1, float y1, float x2, float y2, float r) {
        CGContextAddArcToPoint(_drawingContext, x1, y1, x2, y2, r);
    }

    void CanvasContext2d::isPointInPath(float x, float y) {

    }

    void CanvasContext2d::scale(float scaleWidth, float scaleHeight) {

    }

    void CanvasContext2d::rotate(float angle) {

    }

    void CanvasContext2d::translate(float x, float y) {

    }

    void CanvasContext2d::transform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2d::setTransform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2d::setFont(const std::string &font) {

    }

    void CanvasContext2d::setTextAlign() {

    }

    void CanvasContext2d::setTextBaseline() {

    }

    void CanvasContext2d::fillText(float text, float x, float y, float maxWidth) {

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
        CGImageRef image = CGBitmapContextCreateImage(_drawingContext);
        CGContextDrawImage(context->platformContext(), CGRectMake(destRect.origin.x, destRect.origin.y, destRect.size.width, destRect.size.height), image);
    }

    void CanvasContext2d::ensureDrawingContext() {
        if (_drawingContext == nullptr && _contentSize.width && _contentSize.height) {
            size_t width = static_cast<size_t>(_contentSize.width), height = static_cast<size_t>(_contentSize.height);
            _imageData = (uint8_t *) calloc(width * height * 4, sizeof(uint8_t));
            _drawingContext = CGBitmapContextCreate(_imageData, width, height, 8, width * 4,
                    CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
        }
    }

}