//
// Created by neo on 2018/4/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASCONTEXT2DGL_H
#define ATOMCANVASCONTEXT2DGL_H


#include "node/canvas/AtomCanvasContext2D.h"
#include "node/canvas/AtomGCanvasNode.h"

namespace AtomGraphics {

    class GCanvasNode;

    class GCanvasContext2D : public CanvasContext2D {

    public:
        GCanvasContext2D(GCanvasNode *node);

        void setFillStyle(const Color4F &color) override;

        void setFillStyle(const AtomCanvasPattern *pattern) override;

        void setFillStyle(const AtomCanvasLinearGradient *gradient) override;

        void setFillStyle(const AtomCanvasRadialGradient *gradient) override;

        void setStrokeStyle(const Color4F &color) override;

        void setStrokeStyle(const CanvasPattern &pattern) override;

        void setStrokeStyle(const CanvasGradient &gradient) override;

        void setShadowColor(const Color4F &color) override;

        void setShadowBlur(const int shadowBlur) override;

        void setShadowOffsetX(const float offsetX) override;

        void setShadowOffsetY(const float offsetY) override;

        AtomCanvasPattern *createPattern(const std::string &style, const AtomCanvasImage *image) override;

        AtomCanvasLinearGradient *createLinearGradient(float x0, float y0, float x1, float y1) override;

        AtomCanvasRadialGradient *createRadialGradient(float x0, float y0, float r0, float x1, float y1, float r1) override;

        void setLineCap(const std::string lineCap) override;

        void setLineJoin(const std::string lineJoin) override;

        void setLineWidth(double width) override;

        void setMiterLimit(float limit) override;

        void setRect(float x, float y, float width, float height) override;

        void fillRect(float x, float y, float width, float height) override;

        void strokeRect(float x, float y, float width, float height) override;

        void clearRect(float x, float y, float width, float height) override;

        void fill() override;

        void stroke() override;

        void beginPath() override;

        void moveTo(double x, double y) override;

        void closePath() override;

        void lineTo(double x, double y) override;

        void clip() override;

        void quadraticCurveTo(float cpx, float cpy, float x, float y) override;

        void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) override;

        void arc(double x, double y, double r, double sAngle, double eAngle, bool counterclockwise) override;

        void arcTo(float x1, float y1, float x2, float y2, float r) override;

        void isPointInPath(float x, float y) override;

        void scale(float scaleWidth, float scaleHeight) override;

        void rotate(double angle) override;

        void translate(float x, float y) override;

        void transform(float a, float b, float c, float d, float e, float f) override;

        void setTransform(double a, double b, double c, double d, double e, double f) override;

        void setFont(const std::string font) override;

        void setTextAlign(const std::string textAlign) override;

        void setTextBaseline(const std::string textBaseline) override;

        void fillText(std::string text, double x, double y, float maxWidth) override;

        void strokeText(const std::string text, double x, double y, float maxWidth) override;

        float measureText(const std::string &text) override;

        void drawImage(AtomCanvasImage *image, float x, float y) override;

        void drawImage(AtomCanvasImage *image, float x, float y, float width, float height) override;

        void drawImage(AtomCanvasImage *image, float sx, float sy, float swidth, float sheight, float x, float y, float width, float height) override;

        void drawImage(CanvasNode *canvasNode) override;

        AtomCanvasImageData *createImageData(int width, int height) override;

        AtomCanvasImageData *createImageData(AtomCanvasImageData *imageData) override;

        void *getImageData(int x, int y, int width, int height) override;

        void putImageData(AtomCanvasImageData *imageData, int x, int y, int srcWidth, int srcHeight, int destWidth, int destHeight) override;

        void setGlobalAlpha(double number) override;

        void setGlobalCompositeOperation(const std::string operation) override;

        void save() override;

        void restore() override;

        void createEvent() override;

        void *toDataURL() override;

        void bindImageTexture(const std::string &src, int imageId);

        std::string &getImageDataResult(const std::string &componentId);

        void setDevicePixelRatio(float devicePixelRatio) {
            _devicePixelRatio = devicePixelRatio;
        };

        const std::string flushCommands() {
            std::string commands = _renderCommandLine;
            _renderCommandLine.clear();
            return commands;
        }

    private:

        std::string _renderCommandLine;
        std::string _renderCommandFragment = "";
        GCanvasNode *_gCanvasNode;

        std::string _currentFont;
        float _devicePixelRatio;

        const std::string safeText(const std::string &text);

        void addCommand(const std::string &command);

        void scheduleFlushIfNeed();

        void setContentDirty();
    };

}

#endif //ATOMCANVASCONTEXT2DGL_H
