//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//


#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include "AtomCanvasContext2dCG.h"
#include <CoreGraphics/CGBitmapContext.h>

namespace AtomGraphics {

    void CanvasContext2dCG::setFillStyle(const Color4F &color) {
        const CGFloat colorComponents[4] = {color.r, color.g, color.b, color.a};
        // TODO: colorSpace should be cached
        CGContextSetFillColorWithColor(_drawingContext, CGColorCreate(CGColorSpaceCreateDeviceRGB(), colorComponents));
    }

    void CanvasContext2dCG::setFillStyle(const CanvasPattern &pattern) {
    }

    void CanvasContext2dCG::setFillStyle(const CanvasGradient &gradient) {
    }

    void CanvasContext2dCG::setStrokeStyle(const Color4F &color) {
        const CGFloat colorComponents[4] = {color.r, color.g, color.b, color.a};
        CGContextSetStrokeColor(_drawingContext, colorComponents);
    }

    void CanvasContext2dCG::setStrokeStyle(const CanvasPattern &pattern) {
    }

    void CanvasContext2dCG::setStrokeStyle(const CanvasGradient &gradient) {
    }

    void CanvasContext2dCG::setShadowColor() {
    }

    void CanvasContext2dCG::setShadowBlur() {
    }

    void CanvasContext2dCG::setShadowOffsetX() {
    }

    void CanvasContext2dCG::setShadowOffsetY() {
    }

    CanvasPattern *CanvasContext2dCG::createPattern() {
        return nullptr;
    }

    CanvasGradient *CanvasContext2dCG::createLinearGradient(float x0, float y0, float x1, float y1) {
        return nullptr;
    }

    void *CanvasContext2dCG::createRadialGradient() {
        return nullptr;
    }

    void CanvasContext2dCG::setLineCap(const std::string lineCap) {

    }

    void CanvasContext2dCG::setLineJoin(const std::string lineJoin) {

    }

    void CanvasContext2dCG::setLineWidth(float width) {

    }

    void CanvasContext2dCG::setMiterLimit(float limit) {

    }

    void CanvasContext2dCG::setRect(float x, float y, float width, float height) {

    }

    void CanvasContext2dCG::fillRect(float x, float y, float width, float height) {

    }

    void CanvasContext2dCG::strokeRect(float x, float y, float width, float height) {

    }

    void CanvasContext2dCG::clearRect(float x, float y, float width, float height) {

    }

    void CanvasContext2dCG::fill() {
        CGContextBeginPath(_drawingContext);
        CGContextAddPath(_drawingContext, _path);
        CGContextFillPath(_drawingContext);
    }

    void CanvasContext2dCG::stroke() {
        if (_path && !CGPathIsEmpty(_path)) {
            CGContextBeginPath(_drawingContext);
            CGContextAddPath(_drawingContext, _path);
            CGContextStrokePath(_drawingContext);
        }
    }

    void CanvasContext2dCG::beginPath() {
        if (!_path)
            return;

        CGPathRelease(_path);
        _path = CGPathCreateMutable();
    }

    void CanvasContext2dCG::moveTo(float x, float y) {

    }

    void CanvasContext2dCG::closePath() {

    }

    void CanvasContext2dCG::lineTo(float x, float y) {

    }

    void CanvasContext2dCG::clip() {

    }

    void CanvasContext2dCG::quadraticCurveTo(float cpx, float cpy, float x, float y) {

    }

    void CanvasContext2dCG::arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise) {
        CGPathAddArc(ensurePlatformPath(), nullptr, x, y, r, sAngle, eAngle, counterclockwise);
    }

    void CanvasContext2dCG::arcTo(float x1, float y1, float x2, float y2, float r) {
        CGContextAddArcToPoint(_drawingContext, x1, y1, x2, y2, r);
    }

    void CanvasContext2dCG::isPointInPath(float x, float y) {

    }

    void CanvasContext2dCG::scale(float scaleWidth, float scaleHeight) {

    }

    void CanvasContext2dCG::rotate(double angle) {

    }

    void CanvasContext2dCG::translate(float x, float y) {

    }

    void CanvasContext2dCG::transform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2dCG::setTransform(float a, float b, float c, float d, float e, float f) {

    }

    void CanvasContext2dCG::setFont(const std::string font) {

    }

    void CanvasContext2dCG::setTextAlign() {

    }

    void CanvasContext2dCG::setTextBaseline() {

    }

    void CanvasContext2dCG::fillText(std::string text, float x, float y, float maxWidth) {

    }

    void CanvasContext2dCG::strokeText(const std::string &text, float x, float y, float maxWidth) {

    }

    void *CanvasContext2dCG::measureText(const std::string &text) {
        return nullptr;
    }

    void *CanvasContext2dCG::drawImage(ImageBuffer *imageBuffer, float x, float y) {
        return nullptr;
    }

    void *CanvasContext2dCG::drawImage(ImageBuffer *imageBuffer, float x, float y, float with, float height) {
        return nullptr;
    }

    void *CanvasContext2dCG::drawImage(ImageBuffer *imageBuffer, float sx, float sy, float swith, float sheight, float x, float y, float width, float height) {
        return nullptr;
    }

    void *CanvasContext2dCG::createImageData(float width, float height) {
        return nullptr;
    }

    void *CanvasContext2dCG::createImageData(void *imageData) {
        return nullptr;
    }

    void CanvasContext2dCG::setGlobalAlpha(float number) {

    }

    void CanvasContext2dCG::setGlobalCompositeOperation() {

    }

    void CanvasContext2dCG::save() {

    }

    void CanvasContext2dCG::restore() {

    }

    void CanvasContext2dCG::createEvent() {

    }

    void *CanvasContext2dCG::getContext() {
        return nullptr;
    }

    void *CanvasContext2dCG::toDataURL() {
        return nullptr;
    }

    void CanvasContext2dCG::drawConsuming(const GraphicsContext *context, Rect destRect) {
        CGImageRef image = CGBitmapContextCreateImage(_drawingContext);
        CGContextDrawImage(context->platformContext(), CGRectMake(destRect.origin.x, destRect.origin.y, destRect.size.width, destRect.size.height), image);
    }

    void CanvasContext2dCG::ensureDrawingContext() {
        if (_drawingContext == nullptr && _contentSize.width && _contentSize.height) {
            size_t width = static_cast<size_t>(_contentSize.width), height = static_cast<size_t>(_contentSize.height);
            _imageData = (uint8_t *) calloc(width * height * 4, sizeof(uint8_t));
            _drawingContext = CGBitmapContextCreate(_imageData, width, height, 8, width * 4,
                    CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
        }
    }

    PlatformPath CanvasContext2dCG::ensurePlatformPath() {
        if (!_path) {
            _path = CGPathCreateMutable();;
        }

        return _path;
    }

}

#endif