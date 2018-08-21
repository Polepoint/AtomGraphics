//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//


#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#import "AtomCanvasContext2dCG.h"
#import <CoreGraphics/CGBitmapContext.h>

namespace AtomGraphics {

    CanvasContext2dCG::CanvasContext2dCG(CanvasNode *node) : CanvasContext2D(node) {
    };

    void CanvasContext2dCG::setFillStyle(const Color4F &color) {
        m_dirty = true;

        const CGFloat colorComponents[4] = {color.r, color.g, color.b, color.a};
        // TODO: colorSpace should be cached
//        CGContextSetFillColorWithColor(_drawingContext, CGColorCreate(CGColorSpaceCreateDeviceRGB(), colorComponents));
    }

    void CanvasContext2dCG::setFillStyle(const AtomCanvasPattern *pattern) {
    }

    void CanvasContext2dCG::setFillStyle(const AtomGraphics::AtomCanvasLinearGradient *gradient) {
    }

    void CanvasContext2dCG::setFillStyle(const AtomGraphics::AtomCanvasRadialGradient *gradient) {
    }

    void CanvasContext2dCG::setStrokeStyle(const Color4F &color) {
        m_dirty = true;

        const CGFloat colorComponents[4] = {color.r, color.g, color.b, color.a};
//        CGContextSetStrokeColor(_drawingContext, colorComponents);
    }

    void CanvasContext2dCG::setStrokeStyle(const CanvasPattern &pattern) {
    }

    void CanvasContext2dCG::setStrokeStyle(const CanvasGradient &gradient) {
    }

    void CanvasContext2dCG::setShadowColor(const Color4F &color) {
    }

    void CanvasContext2dCG::setShadowBlur(const int shadowBlur) {
    }

    void CanvasContext2dCG::setShadowOffsetX(const float offsetX) {
    }

    void CanvasContext2dCG::setShadowOffsetY(const float offsetY) {
    }

    AtomCanvasPattern *CanvasContext2dCG::createPattern(const std::string &style, const AtomCanvasImage *image) {
        return nullptr;
    }

    AtomCanvasLinearGradient *CanvasContext2dCG::createLinearGradient(float x0, float y0, float x1, float y1) {
        return nullptr;
    }

    AtomCanvasRadialGradient *CanvasContext2dCG::createRadialGradient(float x0, float y0, float r0, float x1, float y1, float r1) {
        return nullptr;
    }

    void CanvasContext2dCG::setLineCap(const std::string lineCap) {

    }

    void CanvasContext2dCG::setLineJoin(const std::string lineJoin) {

    }

    void CanvasContext2dCG::setLineWidth(double width) {

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
//        CGContextBeginPath(_drawingContext);
//        CGContextAddPath(_drawingContext, _path);
//        CGContextFillPath(_drawingContext);
//        if (m_dirty) {
//            m_flushController->flushLayers();
//            m_dirty = false;
//        }
    }

    void CanvasContext2dCG::stroke() {
//        if (_path && !CGPathIsEmpty(_path)) {
//            CGContextBeginPath(_drawingContext);
//            CGContextAddPath(_drawingContext, _path);
//            CGContextStrokePath(_drawingContext);
//            if (m_dirty) {
//                m_flushController->flushLayers();
//                m_dirty = false;
//            }
//        }
    }

    void CanvasContext2dCG::beginPath() {
//        if (!_path)
//            return;
//
//        CGPathRelease(_path);
//        _path = CGPathCreateMutable();
    }

    void CanvasContext2dCG::moveTo(double x, double y) {

    }

    void CanvasContext2dCG::closePath() {

    }

    void CanvasContext2dCG::lineTo(double x, double y) {

    }

    void CanvasContext2dCG::clip() {

    }

    void CanvasContext2dCG::quadraticCurveTo(float cpx, float cpy, float x, float y) {

    }

    void CanvasContext2dCG::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {

    }

    void CanvasContext2dCG::arc(double x, double y, double r, double sAngle, double eAngle, bool counterclockwise) {
//        m_dirty = true;
//        CGPathAddArc(ensurePlatformPath(), nullptr, x, y, r, sAngle, eAngle, counterclockwise);
    }

    void CanvasContext2dCG::arcTo(float x1, float y1, float x2, float y2, float r) {
//        m_dirty = true;
//        CGContextAddArcToPoint(_drawingContext, x1, y1, x2, y2, r);
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

    void CanvasContext2dCG::setTransform(double a, double b, double c, double d, double e, double f) {

    }

    void CanvasContext2dCG::setFont(const std::string font) {

    }

    void CanvasContext2dCG::setTextAlign(const std::string textAlign) {

    }

    void CanvasContext2dCG::setTextBaseline(const std::string textBaseline) {

    }

    void CanvasContext2dCG::fillText(std::string text, double x, double y, float maxWidth) {

    }

    void CanvasContext2dCG::strokeText(const std::string text, double x, double y, float maxWidth) {

    }

    float CanvasContext2dCG::measureText(const std::string &text) {
        return 0;
    }

    void CanvasContext2dCG::drawImage(AtomCanvasImage *image, float x, float y) {
    }

    void CanvasContext2dCG::drawImage(AtomCanvasImage *image, float x, float y, float width, float height) {
    }

    void CanvasContext2dCG::drawImage(AtomCanvasImage *image, float sx, float sy, float swidth, float sheight, float x, float y, float width, float height) {
    }

    AtomCanvasImageData *CanvasContext2dCG::createImageData(int width, int height) {
        return nullptr;
    }

    AtomCanvasImageData *CanvasContext2dCG::createImageData(AtomCanvasImageData *imageData) {
        return nullptr;
    }

    void *CanvasContext2dCG::getImageData(int x, int y, int width, int height) {
        return nullptr;
    }

    void CanvasContext2dCG::putImageData(AtomCanvasImageData *imageData, int x, int y, int srcWidth, int srcHeight, int destWidth, int destHeight) {

    }

    void CanvasContext2dCG::setGlobalAlpha(double number) {

    }

    void CanvasContext2dCG::setGlobalCompositeOperation(const std::string operation) {

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
//         TODO: Use CGImageCreate()
//        CGImageRef image = CGBitmapContextCreateImage(_drawingContext);
//        CGContextDrawImage(context->platformContext(), CGRectMake(destRect.origin.x, destRect.origin.y, destRect.size.width, destRect.size.height), image);
    }

    void CanvasContext2dCG::ensureDrawingContext() {
//        if (_drawingContext == nullptr && _contentSize.width && _contentSize.height) {
//            size_t width = static_cast<size_t>(_contentSize.width), height = static_cast<size_t>(_contentSize.height);
//            _imageData = (uint8_t *) calloc(width * height * 4, sizeof(uint8_t));
//            _drawingContext = CGBitmapContextCreate(_imageData, width, height, 8, width * 4,
//                    CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
//        }
    }
//
//    GraphicsContext *CanvasContext2dCG::ensurePlatformPath() {
////        if (!_path) {
////            _path = CGPathCreateMutable();;
////        }
////
////        return _path;
//        return nullptr;
//    }

    void CanvasContext2dCG::drawImage(CanvasNode *canvasNode) {
        //(CanvasNodeCG *)canvasNode
    }

}

#endif