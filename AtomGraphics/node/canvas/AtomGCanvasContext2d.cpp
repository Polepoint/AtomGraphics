//
// Created by neo on 2018/4/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGCanvasContext2d.h"
#include <string>

using namespace std;

namespace AtomGraphics {

    void GCanvasContext2d::setFillStyle(const Color4F &color) {
        int r = (int) (color.r * 255);
        int g = (int) (color.g * 255);
        int b = (int) (color.b * 255);
        float a = color.a;
        string const &cmd = string("Frgb(") + to_string(r) + string(",")
                + to_string(g) + string(",") + to_string(b) + string(",")
                + to_string(a) + string(")");
        addCommand(cmd);
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
        //todo require canvas shadow color api
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
        //todo executeCmd , then get a p
        return nullptr;
    }

    void *GCanvasContext2d::createRadialGradient() {
        return nullptr;
    }

    void GCanvasContext2d::setLineCap(const string lineCap) {
        //lineCap includes "butt" "round" "square"
        string const &cmd = string("C") + lineCap;
        addCommand(cmd);
    }

    void GCanvasContext2d::setLineJoin(const string lineJoin) {
        //lineJoin includes "miter" "bevel" "round"
        string const &cmd = string("J") + lineJoin;
        addCommand(cmd);
    }

    void GCanvasContext2d::setLineWidth(float width) {
        string const &cmd = string("W") + to_string(width);
        addCommand(cmd);
    }

    void GCanvasContext2d::setMiterLimit(float limit) {
        string const &cmd = string("M") + to_string(limit);
        addCommand(cmd);
    }

    void GCanvasContext2d::setRect(float x, float y, float width, float height) {
        string const &cmd = string("w") + to_string(x) + string(",") + to_string(y) + string(",") + to_string(width) + string(",") + to_string(height);
        addCommand(cmd);
    }

    void GCanvasContext2d::fillRect(float x, float y, float width, float height) {
        string const &cmd = string("n") + to_string(x) + string(",") + to_string(y) + string(",") + to_string(width) + string(",") + to_string(height);
        addCommand(cmd);
    }

    void GCanvasContext2d::strokeRect(float x, float y, float width, float height) {
        string const &cmd = string("s") + to_string(x) + string(",") + to_string(y) + string(",") + to_string(width) + string(",") + to_string(height);
        addCommand(cmd);
    }

    void GCanvasContext2d::clearRect(float x, float y, float width, float height) {
        string const &cmd = string("c") + to_string(x) + string(",") + to_string(y) + string(",") + to_string(width) + string(",") + to_string(height);
        addCommand(cmd);
    }

    void GCanvasContext2d::fill() {
        string const &cmd = string("L");
        addCommand(cmd);
    }

    void GCanvasContext2d::stroke() {
        string const &cmd = string("x");
        addCommand(cmd);
    }

    void GCanvasContext2d::beginPath() {
        string const &cmd = string("b");
        addCommand(cmd);
    }

    void GCanvasContext2d::moveTo(float x, float y) {
        string const &cmd = string("g") + to_string(x) + string(",") + to_string(y);
        addCommand(cmd);
    }

    void GCanvasContext2d::closePath() {
        string const &cmd = string("o");
        addCommand(cmd);
    }

    void GCanvasContext2d::lineTo(float x, float y) {
        string const &cmd = string("i") + to_string(x) + string(",") + to_string(y);
        addCommand(cmd);
    }

    void GCanvasContext2d::clip() {
        string const &cmd = string("p");
        addCommand(cmd);
    }

    void GCanvasContext2d::quadraticCurveTo(float cpx, float cpy, float x, float y) {
        string const &cmd = string("u") + to_string(cpx) + string(",")+ to_string(cpy) + string(",")+ to_string(x) + string(",")+ to_string(y);
        addCommand(cmd);
    }

    void GCanvasContext2d::arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise) {
        char buffer[100] = {0};
        sprintf(buffer, "y%f,%f,%f,%f,%f,%d", x, y, r, sAngle, eAngle, counterclockwise);
        addCommand(std::string(buffer));
        //todo 改成string
    }

    void GCanvasContext2d::arcTo(float x1, float y1, float x2, float y2, float r) {
        string const &cmd = string("h") + to_string(x1) + string(",")+ to_string(y1) + string(",")+ to_string(x2) + string(",")+ to_string(y2) + string(",")+ to_string(r);
        addCommand(cmd);
    }

    void GCanvasContext2d::isPointInPath(float x, float y) {
        //todo GCanvas 未提供对应api
        // 缺少当前路径的一个副本，用于判断point与当前路径的一个位置关系，返回值应是bool？
    }

    void GCanvasContext2d::scale(float scaleWidth, float scaleHeight) {
        string const &cmd = string("k") + to_string(scaleWidth) + string(",")  + to_string(scaleHeight);
        addCommand(cmd);
    }

    void GCanvasContext2d::rotate(double angle) {
        string const &cmd = string("r") + to_string(angle);
        addCommand(cmd);
    }

    void GCanvasContext2d::translate(float x, float y) {
        string const &cmd = string("l") + to_string(x) + string(",")  + to_string(y);
        addCommand(cmd);
    }

    void GCanvasContext2d::transform(float a, float b, float c, float d, float e, float f) {
        string const &cmd = string("f") + to_string(a) + string(",") + to_string(b) + string(",")+ to_string(c) + string(",")+ to_string(d) + string(",")+ to_string(e) + string(",")+ to_string(f);
        addCommand(cmd);
    }

    void GCanvasContext2d::setTransform(float a, float b, float c, float d, float e, float f) {
        string const &cmd = string("t") + to_string(a) + string(",") + to_string(b) + string(",")+ to_string(c) + string(",")+ to_string(d) + string(",")+ to_string(e) + string(",")+ to_string(f);
        addCommand(cmd);
    }

    void GCanvasContext2d::setFont(const string font) {
        string const &cmd = string("j") + font;
        addCommand(cmd);
    }

    void GCanvasContext2d::setTextAlign() {

    }

    void GCanvasContext2d::setTextBaseline() {

    }

    void GCanvasContext2d::fillText(string text, float x, float y, float maxWidth) {
        string const &cmd = string("T") + text + string(",") + to_string(x) + string(",") + to_string(y) + string(",") + to_string(maxWidth);
        addCommand(cmd);
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