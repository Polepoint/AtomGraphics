//
// Created by neo on 2018/4/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGCanvasContext2d.h"

namespace AtomGraphics {

    void GCanvasContext2d::setFillStyle(const Color4F &color) {
        char buffer[40] = {0};
        sprintf(buffer, "Frgb(%d,%d,%d,%f)", (int) (color.r * 255), (int) (color.g * 255), (int) (color.b * 255), color.a);
        addCommand(std::string(buffer));
    }

    void GCanvasContext2d::setFillStyle(const CanvasPattern &pattern) {

    }

    void GCanvasContext2d::setFillStyle(const CanvasGradient &gradient) {

    }

    void GCanvasContext2d::setStrokeStyle(const Color4F &color) {
        char buffer[40] = {0};
        sprintf(buffer, "Srgb(%d,%d,%d,%f)", (int) (color.r * 255), (int) (color.g * 255), (int) (color.b * 255), color.a);
        addCommand(std::string(buffer));
    }

    void GCanvasContext2d::setStrokeStyle(const CanvasPattern &pattern) {

    }

    void GCanvasContext2d::setStrokeStyle(const CanvasGradient &gradient) {

    }

    void GCanvasContext2d::setShadowColor() {

    }

    void GCanvasContext2d::setShadowBlur() {

    }

    void GCanvasContext2d::setShadowOffsetX() {

    }

    void GCanvasContext2d::setShadowOffsetY() {

    }

    CanvasPattern *GCanvasContext2d::createPattern() {
        return nullptr;
    }

    CanvasGradient *GCanvasContext2d::createLinearGradient(float x0, float y0, float x1, float y1) {
        return nullptr;
    }

    void *GCanvasContext2d::createRadialGradient() {
        return nullptr;
    }

    void GCanvasContext2d::setLineCap() {

    }

    void GCanvasContext2d::setLineJoin() {
        ;
    }

    void GCanvasContext2d::setLineWidth() {

    }

    void GCanvasContext2d::setMiterLimit() {

    }

    void GCanvasContext2d::setRect(float x, float y, float width, float height) {

    }

    void GCanvasContext2d::fillRect() {

    }

    void GCanvasContext2d::strokeRect(float x, float y, float width, float height) {

    }

    void GCanvasContext2d::clearRect(float x, float y, float width, float height) {

    }

    void GCanvasContext2d::fill() {
        addCommand("L");
    }

    void GCanvasContext2d::stroke() {
        addCommand("x");
    }

    void GCanvasContext2d::beginPath() {

    }

    void GCanvasContext2d::moveTo(float x, float y) {

    }

    void GCanvasContext2d::closePath() {

    }

    void GCanvasContext2d::lineTo(float x, float y) {

    }

    void GCanvasContext2d::clip() {

    }

    void GCanvasContext2d::quadraticCurveTo(float cpx, float cpy, float x, float y) {

    }

    void GCanvasContext2d::arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise) {
        char buffer[100] = {0};
        sprintf(buffer, "y%f,%f,%f,%f,%f,%d", x, y, r, sAngle, eAngle, counterclockwise);
        addCommand(std::string(buffer));
    }

    void GCanvasContext2d::arcTo(float x1, float y1, float x2, float y2, float r) {

    }

    void GCanvasContext2d::isPointInPath(float x, float y) {

    }

    void GCanvasContext2d::scale(float scaleWidth, float scaleHeight) {

    }

    void GCanvasContext2d::rotate(float angle) {

    }

    void GCanvasContext2d::translate(float x, float y) {

    }

    void GCanvasContext2d::transform(float a, float b, float c, float d, float e, float f) {

    }

    void GCanvasContext2d::setTransform(float a, float b, float c, float d, float e, float f) {

    }

    void GCanvasContext2d::setFont(const std::string &font) {

    }

    void GCanvasContext2d::setTextAlign() {

    }

    void GCanvasContext2d::setTextBaseline() {

    }

    void GCanvasContext2d::fillText(float text, float x, float y, float maxWidth) {

    }

    void GCanvasContext2d::strokeText(const std::string &text, float x, float y, float maxWidth) {

    }

    void *GCanvasContext2d::measureText(const std::string &text) {
        return nullptr;
    }

    void *GCanvasContext2d::drawImage(ImageBuffer *imageBuffer, float x, float y) {
        return nullptr;
    }

    void *GCanvasContext2d::drawImage(ImageBuffer *imageBuffer, float x, float y, float with, float height) {
        return nullptr;
    }

    void *GCanvasContext2d::drawImage(ImageBuffer *imageBuffer, float sx, float sy, float swith, float sheight, float x, float y, float width, float height) {
        return nullptr;
    }

    void *GCanvasContext2d::createImageData(float width, float height) {
        return nullptr;
    }

    void *GCanvasContext2d::createImageData(void *imageData) {
        return nullptr;
    }

    void GCanvasContext2d::setGlobalAlpha(float number) {

    }

    void GCanvasContext2d::setGlobalCompositeOperation() {

    }

    void GCanvasContext2d::save() {

    }

    void GCanvasContext2d::restore() {

    }

    void GCanvasContext2d::createEvent() {

    }

    void *GCanvasContext2d::toDataURL() {
        return nullptr;
    }

    void GCanvasContext2d::setComponentID(const std::string &componentID) {
        _componentId = componentID;
    }
}