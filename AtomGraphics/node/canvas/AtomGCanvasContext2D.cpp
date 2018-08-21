//
// Created by neo on 2018/4/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "node/canvas/AtomGCanvasContext2D.h"
#include "base/AtomEncode.h"
#include "node/canvas/AtomGCanvasNode.h"
#include <string>
#include <stdlib.h>

using namespace std;

namespace AtomGraphics {


    GCanvasContext2D::GCanvasContext2D(GCanvasNode *node) : CanvasContext2D(node) {
        _gCanvasNode = node;
    }

    void GCanvasContext2D::setFillStyle(const Color4F &color) {
        int r = (int) (color.r * 255);
        int g = (int) (color.g * 255);
        int b = (int) (color.b * 255);
        float a = color.a;

        char rBuffer[10] = {'0'};
        char gBuffer[10] = {'0'};
        char bBuffer[10] = {'0'};
        char aBuffer[10] = {'0'};
        sprintf(rBuffer, "%d", r);
        sprintf(gBuffer, "%d", g);
        sprintf(bBuffer, "%d", b);
        sprintf(aBuffer, "%g", a);

        string const &cmd = string("Frgb(") + rBuffer + string(",")
                + gBuffer + string(",") + bBuffer + string(",")
                + aBuffer + string(")");
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setFillStyle(const AtomCanvasPattern *pattern) {
        int imageId = pattern->getTextureListId();
        bindImageTexture(pattern->getImageSrc(), imageId);
        char imageIdBuffer[10] = {'0'};
        sprintf(imageIdBuffer, "%d", imageId);

        string const &cmd = string("G") + imageIdBuffer + "," + pattern->getPattern();
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setFillStyle(const AtomCanvasLinearGradient *gradient) {
        AtomCanvasLinearGradient::AtomPoint st = gradient->getStartPos();
        AtomCanvasLinearGradient::AtomPoint ed = gradient->getEndPos();
        string colorStopCmd = string("");
        int stopCount = gradient->getStopCount();
        for (int i = 0; i < stopCount; i++) {
            AtomCanvasLinearGradient::ColorStop const *colorStop = gradient->getColorStop(i);
            //append to color stop cmd
            char stopBuffer[10] = {'0'};
            sprintf(stopBuffer, "%g", colorStop->pos);
            colorStopCmd = colorStopCmd + string(",") + stopBuffer + string(",") + colorStop->color;
        }

        char stXBuffer[10] = {'0'};
        char stYBuffer[10] = {'0'};
        char edXBuffer[10] = {'0'};
        char edYBuffer[10] = {'0'};
        sprintf(stXBuffer, "%g", st.x);
        sprintf(stXBuffer, "%g", st.y);
        sprintf(edXBuffer, "%g", ed.x);
        sprintf(edYBuffer, "%g", ed.y);

        char stCountBuffer[10] = {'0'};
        sprintf(stCountBuffer, "%d", stopCount);

        string const &cmd = string("D") + stXBuffer + string(",") + stYBuffer + string(",")
                + edXBuffer + string(",") + edYBuffer + string(",") + stCountBuffer + colorStopCmd;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setFillStyle(const AtomCanvasRadialGradient *gradient) {
        const float *st = gradient->getStartPos();
        const float *ed = gradient->getEndPos();

        char st0Buffer[10] = {'0'};
        char st1Buffer[10] = {'0'};
        char st2Buffer[10] = {'0'};
        char ed0Buffer[10] = {'0'};
        char ed1Buffer[10] = {'0'};
        char ed2Buffer[10] = {'0'};
        sprintf(st0Buffer, "%g", st[0]);
        sprintf(st1Buffer, "%g", st[1]);
        sprintf(st2Buffer, "%g", st[2]);
        sprintf(ed0Buffer, "%g", ed[0]);
        sprintf(ed1Buffer, "%g", ed[1]);
        sprintf(ed2Buffer, "%g", ed[2]);

        string colorStopCmd = string("");
        int stopCount = gradient->getStopCount();
        for (int i = 0; i < stopCount; i++) {
            AtomCanvasRadialGradient::ColorStop const *colorStop = gradient->getColorStop(i);
            //append to color stop cmd
            char stopBuffer[10] = {'0'};
            sprintf(stopBuffer, "%g", colorStop->pos);

            colorStopCmd = colorStopCmd + "," + stopBuffer + "," + colorStop->color;
        }
        char stCountBuffer[10] = {'0'};
        sprintf(stCountBuffer, "%d", stopCount);

        string const &cmd = string("H") + st0Buffer + string(",") + st1Buffer + string(",") + st2Buffer + string(",")
                + ed0Buffer + string(",") + ed1Buffer + string(",") + ed2Buffer + string(",")
                + stCountBuffer + colorStopCmd;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setStrokeStyle(const Color4F &color) {
        int r = (int) (color.r * 255);
        int g = (int) (color.g * 255);
        int b = (int) (color.b * 255);
        float a = color.a;

        char rBuffer[10] = {'0'};
        char gBuffer[10] = {'0'};
        char bBuffer[10] = {'0'};
        char aBuffer[10] = {'0'};
        sprintf(rBuffer, "%d", r);
        sprintf(gBuffer, "%d", g);
        sprintf(bBuffer, "%d", b);
        sprintf(aBuffer, "%g", a);

        string const &cmd = string("Srgb(") + rBuffer + string(",")
                + gBuffer + string(",") + bBuffer + string(",")
                + aBuffer + string(")");
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setStrokeStyle(const CanvasPattern &pattern) {

    }

    void GCanvasContext2D::setStrokeStyle(const CanvasGradient &gradient) {

    }

    void GCanvasContext2D::setShadowColor(const Color4F &color) {
        int r = (int) (color.r * 255);
        int g = (int) (color.g * 255);
        int b = (int) (color.b * 255);
        float a = color.a;

        char rBuffer[10] = {'0'};
        char gBuffer[10] = {'0'};
        char bBuffer[10] = {'0'};
        char aBuffer[10] = {'0'};
        sprintf(rBuffer, "%d", r);
        sprintf(gBuffer, "%d", g);
        sprintf(bBuffer, "%d", b);
        sprintf(aBuffer, "%g", a);

        string const &cmd = string("Irgb(") + rBuffer + string(",")
                + gBuffer + string(",") + bBuffer + string(",")
                + aBuffer + string(")");
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setShadowBlur(const int shadowBlur) {
        char blurBuffer[10] = {'0'};
        sprintf(blurBuffer, "%d", shadowBlur);
        string const &cmd = string("Q") + blurBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setShadowOffsetX(const float offsetX) {
        char offsetBuffer[10] = {'0'};
        sprintf(offsetBuffer, "%g", offsetX);

        string const &cmd = string("K") + offsetBuffer;
        addCommand(cmd);
        scheduleFlushIfNeed();
        setContentDirty();
    }

    void GCanvasContext2D::setShadowOffsetY(const float offsetY) {
        char offsetBuffer[10] = {'0'};
        sprintf(offsetBuffer, "%g", offsetY);

        string const &cmd = string("O") + offsetBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    AtomCanvasPattern *GCanvasContext2D::createPattern(const std::string &style, const AtomCanvasImage *image) {
        AtomCanvasPattern *canvasPattern = new AtomCanvasPattern(style, image);
        return canvasPattern;
    }

    AtomCanvasLinearGradient *GCanvasContext2D::createLinearGradient(float x0, float y0, float x1, float y1) {
        AtomCanvasLinearGradient::AtomPoint st, ed;
        st.x = x0;
        st.y = y0;
        ed.x = x1;
        ed.y = y1;
        AtomCanvasLinearGradient *atomLinearGradient = new AtomCanvasLinearGradient(st, ed);
        return atomLinearGradient;
    }

    AtomCanvasRadialGradient *GCanvasContext2D::createRadialGradient(float x0, float y0, float r0, float x1, float y1, float r1) {
        float st[3], ed[3];
        st[0] = x0;
        st[1] = y0;
        st[2] = r0;
        ed[0] = x1;
        ed[1] = y1;
        ed[2] = r1;
        AtomCanvasRadialGradient *atomRadialGradient = new AtomCanvasRadialGradient(st, ed);
        return atomRadialGradient;
    }

    void GCanvasContext2D::setLineCap(const string lineCap) {
        //lineCap includes "butt" "round" "square"
        string const &cmd = string("C") + lineCap;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setLineJoin(const string lineJoin) {
        //lineJoin includes "miter" "bevel" "round"
        string const &cmd = string("J") + lineJoin;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setLineWidth(double width) {
        char widthBuffer[10] = {'0'};
        sprintf(widthBuffer, "%g", width);

        string const &cmd = string("W") + widthBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setMiterLimit(float limit) {
        char limitBuffer[10] = {'0'};
        sprintf(limitBuffer, "%g", limit);

        string const &cmd = string("M") + limitBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setRect(float x, float y, float width, float height) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(widthBuffer, "%g", width);
        sprintf(heightBuffer, "%g", height);

        string const &cmd = string("w") + xBuffer + string(",") + yBuffer + string(",") + widthBuffer + string(",") + heightBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::fillRect(float x, float y, float width, float height) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(widthBuffer, "%g", width);
        sprintf(heightBuffer, "%g", height);

        string const &cmd = string("n") + xBuffer + string(",") + yBuffer + string(",") + widthBuffer + string(",") + heightBuffer;
        addCommand(cmd);
        setContentDirty();
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::strokeRect(float x, float y, float width, float height) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(widthBuffer, "%g", width);
        sprintf(heightBuffer, "%g", height);

        string const &cmd = string("s") + xBuffer + string(",") + yBuffer + string(",") + widthBuffer + string(",") + heightBuffer;
        addCommand(cmd);
        setContentDirty();
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::clearRect(float x, float y, float width, float height) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(widthBuffer, "%g", width);
        sprintf(heightBuffer, "%g", height);

        string const &cmd = string("c") + xBuffer + string(",") + yBuffer + string(",") + widthBuffer + string(",") + heightBuffer;
        addCommand(cmd);
        setContentDirty();
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::fill() {
        string const &cmd = string("L");
        addCommand(cmd);
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::stroke() {
        string const &cmd = string("x");
        addCommand(cmd);
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::beginPath() {
        string const &cmd = string("b");
        addCommand(cmd);
    }

    void GCanvasContext2D::moveTo(double x, double y) {
        char xBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        char yBuffer[10] = {'0'};
        sprintf(yBuffer, "%g", y);

        string const &cmd = string("g") + xBuffer + string(",") + yBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::closePath() {
        string const &cmd = string("o");
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::lineTo(double x, double y) {
        char xBuffer[10] = {'0'};
        sprintf(xBuffer, "%.2f", x);
        char yBuffer[10] = {'0'};
        sprintf(yBuffer, "%.2f", y);

        string const &cmd = string("i") + xBuffer + string(",") + yBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::clip() {
        string const &cmd = string("p");
        addCommand(cmd);
    }

    void GCanvasContext2D::quadraticCurveTo(float cpx, float cpy, float x, float y) {
        char cpxBuffer[10] = {'0'};
        char cpyBuffer[10] = {'0'};
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};

        sprintf(cpxBuffer, "%g", cpx);
        sprintf(cpyBuffer, "%g", cpy);
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);

        string const &cmd = string("u") + cpxBuffer + string(",") + cpyBuffer + string(",")
                + xBuffer + string(",") + yBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
        char cp1xBuffer[10] = {'0'};
        char cp1yBuffer[10] = {'0'};
        char cp2xBuffer[10] = {'0'};
        char cp2yBuffer[10] = {'0'};
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};

        sprintf(cp1xBuffer, "%g", cp1x);
        sprintf(cp1yBuffer, "%g", cp1y);
        sprintf(cp2xBuffer, "%g", cp2x);
        sprintf(cp2yBuffer, "%g", cp2y);
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);

        string const &cmd = string("z") + cp1xBuffer + string(",") + cp1yBuffer + string(",")
                + cp2xBuffer + string(",") + cp2yBuffer + "," + xBuffer + string(",") + yBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::arc(double x, double y, double r, double sAngle, double eAngle, bool counterclockwise) {
        string const &clockStr = counterclockwise ? "1" : "0";
        char xBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        char yBuffer[10] = {'0'};
        sprintf(yBuffer, "%g", y);
        char rBuffer[10] = {'0'};
        sprintf(rBuffer, "%g", r);

        // angle的位数非常重要
        // 会影响到 GCanvas arc 中绘制的逻辑，会导致counterclockwise 其中一个case无法完成绘制
        char sAngleBuffer[10] = {'0'};
        char eAngleBuffer[10] = {'0'};
        sprintf(sAngleBuffer, "%g", sAngle);
        sprintf(eAngleBuffer, "%g", eAngle);

        string const &cmd = string("y") + xBuffer + string(",") + yBuffer + string(",") + rBuffer + string(",")
                + sAngleBuffer + string(",") + eAngleBuffer + string(",") + clockStr;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::arcTo(float x1, float y1, float x2, float y2, float r) {
        char x1Buffer[10] = {'0'};
        char y1Buffer[10] = {'0'};
        char x2Buffer[10] = {'0'};
        char y2Buffer[10] = {'0'};
        char rBuffer[10] = {'0'};
        sprintf(x1Buffer, "%g", x1);
        sprintf(y1Buffer, "%g", y1);
        sprintf(x2Buffer, "%g", x2);
        sprintf(y2Buffer, "%g", y2);
        sprintf(rBuffer, "%g", r);

        string const &cmd = string("h") + x1Buffer + string(",") + y1Buffer + string(",") + x2Buffer + string(",") + y2Buffer
                + string(",") + rBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::isPointInPath(float x, float y) {
        //todo GCanvas 未提供对应api
        // 缺少当前路径的一个副本，用于判断point与当前路径的一个位置关系，返回值应是bool？
    }

    void GCanvasContext2D::scale(float scaleWidth, float scaleHeight) {
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};
        sprintf(widthBuffer, "%g", scaleWidth);
        sprintf(heightBuffer, "%g", scaleHeight);
        string const &cmd = string("k") + widthBuffer + string(",") + heightBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::rotate(double angle) {
        char angleBuffer[10] = {'0'};
        sprintf(angleBuffer, "%g", angle);
        string const &cmd = string("r") + angleBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::translate(float x, float y) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        string const &cmd = string("l") + xBuffer + string(",") + yBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::transform(float a, float b, float c, float d, float e, float f) {
        char aBuffer[10] = {'0'};
        char bBuffer[10] = {'0'};
        char cBuffer[10] = {'0'};
        char dBuffer[10] = {'0'};
        char eBuffer[10] = {'0'};
        char fBuffer[10] = {'0'};

        sprintf(aBuffer, "%g", a);
        sprintf(bBuffer, "%g", b);
        sprintf(cBuffer, "%g", c);
        sprintf(dBuffer, "%g", d);
        sprintf(eBuffer, "%g", e);
        sprintf(fBuffer, "%g", f);

        string const &cmd = string("f") + aBuffer + string(",") + bBuffer + string(",") + cBuffer + string(",")
                + dBuffer + string(",") + eBuffer + string(",") + fBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setTransform(double a, double b, double c, double d, double e, double f) {
        char aBuffer[10] = {'0'};
        char bBuffer[10] = {'0'};
        char cBuffer[10] = {'0'};
        char dBuffer[10] = {'0'};
        char eBuffer[10] = {'0'};
        char fBuffer[10] = {'0'};
        sprintf(aBuffer, "%.3f", a);
        sprintf(bBuffer, "%.3f", b);
        sprintf(cBuffer, "%.3f", c);
        sprintf(dBuffer, "%.3f", d);
        sprintf(eBuffer, "%.3f", e);
        sprintf(fBuffer, "%.3f", f);

        string const &cmd = string("t") + aBuffer + string(",") + bBuffer + string(",") + cBuffer + string(",") + dBuffer
                + string(",") + eBuffer + string(",") + fBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setFont(const string font) {
        if (_currentFont.compare(font) != 0) {
            _currentFont = font;
            string const &cmd = string("j") + _currentFont;
            addCommand(cmd);
        }
    }

    void GCanvasContext2D::setTextAlign(const std::string textAlign) {
        //0 default ,1 end ,2 left ,3 center ,4 right
        int alignValue = 0;
        if (textAlign.compare("end") == 0) {
            alignValue = 1;
        } else if (textAlign.compare("left") == 0) {
            alignValue = 2;
        } else if (textAlign.compare("center") == 0) {
            alignValue = 3;
        } else if (textAlign.compare("right") == 0) {
            alignValue = 4;
        } else {
            alignValue = 0;
        }

        char alignValueBuffer[10] = {'0'};
        sprintf(alignValueBuffer, "%d", alignValue);

        string const &cmd = string("A") + alignValueBuffer;
        addCommand(cmd);
    }

    void GCanvasContext2D::setTextBaseline(const std::string textBaseline) {
        int baselineType = 0;
        if (textBaseline.compare("middle") == 0) {
            baselineType = 1;
        } else if (textBaseline.compare("top") == 0) {
            baselineType = 2;
        } else if (textBaseline.compare("hanging") == 0) {
            baselineType = 3;
        } else if (textBaseline.compare("bottom") == 0) {
            baselineType = 4;
        } else if (textBaseline.compare("ideographic") == 0) {
            baselineType = 4;
        } else {
            baselineType = 0;
        }
        char baselineTypeBuffer[10] = {'0'};
        sprintf(baselineTypeBuffer, "%d", baselineType);

        string const &cmd = string("E") + baselineTypeBuffer;
        addCommand(cmd);
    }

    void GCanvasContext2D::fillText(string text, double x, double y, float maxWidth) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char maxWidthBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(maxWidthBuffer, "%g", maxWidth);

        string const &cmd = string("T") + safeText(text) + string(",") + xBuffer + string(",") + yBuffer + string(",") + maxWidthBuffer;
        addCommand(cmd);
        setContentDirty();
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::strokeText(const string text, double x, double y, float maxWidth) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char maxWidthBuffer[10] = {'0'};
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(maxWidthBuffer, "%g", maxWidth);

        string const &cmd = "U" + safeText(text) + string(",") + xBuffer + "," + yBuffer + "," + maxWidthBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::drawImage(CanvasNode *canvasNode) {
        //暂时认定 只能draw同类的context dom
        GCanvasNode *gCanvasNode = (GCanvasNode *) canvasNode;
        GCanvasContext2D *context2d = (GCanvasContext2D *) gCanvasNode->getContext2d();
        std::string contextCmdLine = context2d->flushCommands();
        std::size_t clearCMDPosition;
        std::size_t semiCMDPosition;
        clearCMDPosition = contextCmdLine.find("c", 0);
        while (clearCMDPosition != std::string::npos
                && (clearCMDPosition == 0 || contextCmdLine[clearCMDPosition - 1] == ';')) {
            semiCMDPosition = contextCmdLine.find(";", clearCMDPosition);
            if (semiCMDPosition == std::string::npos) {
                contextCmdLine = "";
            } else {
                contextCmdLine = contextCmdLine.substr(semiCMDPosition + 1, contextCmdLine.size() - semiCMDPosition - 2);
            }
        }
        //截取clearRect之后的内容
        if (contextCmdLine.size() > 0) {
            addCommand(contextCmdLine);
            if (canvasNode->dirty()) {
                setContentDirty();
                scheduleFlushIfNeed();
            }
        }
    }

    void GCanvasContext2D::drawImage(AtomCanvasImage *image, float x, float y) {
        drawImage(image, x, y, image->getWidth(), image->getHeight());
    }

    void GCanvasContext2D::drawImage(AtomCanvasImage *image, float x, float y, float width, float height) {
        drawImage(image, 0, 0, image->getWidth(), image->getHeight(), x, y, width, height);
    }

    void GCanvasContext2D::drawImage(AtomCanvasImage *image, float sx, float sy, float swidth, float sheight, float x, float y, float width, float height) {
        char sxBuffer[10] = {'0'};
        char syBuffer[10] = {'0'};
        char sWidthBuffer[10] = {'0'};
        char sHeightBuffer[10] = {'0'};
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};

        sprintf(sxBuffer, "%g", sx);
        sprintf(syBuffer, "%g", sy);
        sprintf(sWidthBuffer, "%g", swidth);
        sprintf(sHeightBuffer, "%g", sheight);
        sprintf(xBuffer, "%g", x);
        sprintf(yBuffer, "%g", y);
        sprintf(widthBuffer, "%g", width);
        sprintf(heightBuffer, "%g", height);

        char imageIdBuffer[10] = {'0'};
        sprintf(imageIdBuffer, "%d", image->getId());

        string const &cmd = string("d") + imageIdBuffer + ","
                + sxBuffer + "," + syBuffer + "," + sWidthBuffer + "," + sHeightBuffer + ","
                + xBuffer + "," + yBuffer + "," + widthBuffer + "," + heightBuffer;
        bindImageTexture(image->getSrc(), image->getId());
        addCommand(cmd);
        setContentDirty();
        scheduleFlushIfNeed();
    }


    AtomCanvasImageData *GCanvasContext2D::createImageData(int width, int height) {
        AtomCanvasImageData *imageData = new AtomCanvasImageData(width, height);
        return imageData;
    }

    AtomCanvasImageData *GCanvasContext2D::createImageData(AtomCanvasImageData *imageData) {
        return createImageData(imageData->getWidth(), imageData->getHeight());
    }

    void *GCanvasContext2D::getImageData(int x, int y, int width, int height) {
        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char widthBuffer[10] = {'0'};
        char heightBuffer[10] = {'0'};
        sprintf(xBuffer, "%d", x);
        sprintf(yBuffer, "%d", y);
        sprintf(widthBuffer, "%d", width);
        sprintf(heightBuffer, "%d", height);

        string const &cmd = string("R") + xBuffer + string(",") + yBuffer + string(",") + widthBuffer + string(",")
                + heightBuffer;
        addCommand(cmd);
        //make the layer to render
//        string pixelData = getImageDataResulmat(_componentId);

        //todo 目前这个cmd 处理的机制暂不能满足cmd运行结束完毕的临时现场的导出,如这里的绘制结束后的pixelData
        //仅能通过plugin中的getSyncResult获取到图像流数据的地址，调用的时序问题，不好封装

        //todo 创建一个临时版本的数据 并在putImageData的时候合并数据（需要确认data的身份是否同一）？？一个
        return nullptr;
    }

    void GCanvasContext2D::putImageData(AtomCanvasImageData *imageData, int x, int y, int srcWidth, int srcHeight, int destWidth, int destHeight) {
        int *data = imageData->getData();

        int bufSize = imageData->getHeight() * imageData->getWidth() * 4;

        // transform rgba data (int to char)
        char *srcImgData = (char *) malloc((size_t) bufSize);
        char *pForImgData = srcImgData;

        for (int i = 0; i < bufSize; i += 4, pForImgData += 4) {//将imgData里面的rgba int数据填入到 数据流中
            for (int j = 0; j < 4; j++) {
                pForImgData[j] = (char) data[i + j];
            }
        }

        //do base 64 encode
        string pixelsData = string("");
        pixelsData.resize(Base64EncodeLen(bufSize));
        Base64EncodeBuf((char *) (pixelsData.c_str()), srcImgData, bufSize);

        char xBuffer[10] = {'0'};
        char yBuffer[10] = {'0'};
        char srcWidthBuffer[10] = {'0'};
        char srcHeightBuffer[10] = {'0'};
        char destWidthBuffer[10] = {'0'};
        char destHeightBuffer[10] = {'0'};
        sprintf(xBuffer, "%d", x);
        sprintf(yBuffer, "%d", y);
        sprintf(srcWidthBuffer, "%d", srcWidth);
        sprintf(srcHeightBuffer, "%d", srcHeight);
        sprintf(destWidthBuffer, "%d", destWidth);
        sprintf(destHeightBuffer, "%d", destHeight);

        //append cmd
        string const &cmd = string("P") + xBuffer + "," + yBuffer + "," +
                srcWidthBuffer + "," + srcHeightBuffer + "," + destWidthBuffer + "," + destHeightBuffer + "," + pixelsData;
        addCommand(cmd);
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::setGlobalAlpha(double number) {
        char alphaBuffer[10] = {'0'};
        sprintf(alphaBuffer, "%g", number);
        string const &cmd = string("a") + alphaBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::setGlobalCompositeOperation(const std::string operation) {
        int operationType = 0;
        if (operation.compare("source-over") == 0) {
            operationType = 0;
        } else if (operation.compare("lighter") == 0) {
            operationType = 1;
        } else if (operation.compare("darker") == 0) {
            operationType = 2;
        } else if (operation.compare("destination-out") == 0) {
            operationType = 3;
        } else if (operation.compare("destination-over") == 0) {
            operationType = 4;
        } else if (operation.compare("source-atop") == 0) {
            operationType = 5;
        } else if (operation.compare("xor") == 0) {
            operationType = 6;
        } else if (operation.compare("replace") == 0) {
            operationType = 7;
        } else if (operation.compare("alpha") == 0) {
            operationType = 8;
        } else if (operation.compare("lighten") == 0) {//后期补充的，非GCanvas自带效果
            operationType = 9;
        } else {
            operationType = 0;
        }

        char operationTypeBuffer[10] = {'0'};
        sprintf(operationTypeBuffer, "%d", operationType);
        string const &cmd = string("B") + operationTypeBuffer;
        addCommand(cmd);
        setContentDirty();
    }

    void GCanvasContext2D::save() {
        string const &cmd = string("v");
        addCommand(cmd);
    }

    void GCanvasContext2D::restore() {
        string const &cmd = string("e");
        addCommand(cmd);
        scheduleFlushIfNeed();
    }

    void GCanvasContext2D::createEvent() {

    }

    void *GCanvasContext2D::toDataURL() {
        return nullptr;
    }


    void GCanvasContext2D::addCommand(const std::string &command) {
        _renderCommandFragment += command + ";";
    }

    const std::string GCanvasContext2D::safeText(const std::string &text) {
        //TODO 效果还有问题
        std::string copyText = text;
        std::string value1 = "!";
        std::string value2 = ",";
        std::string value3 = ";";
        if (text.find(value1) < text.length()) {
            copyText = ((copyText).replace(text.find(value1), value1.length(), "!!"));
        }
        if (text.find(value2) < text.length()) {
            copyText = ((copyText).replace(text.find(value2), value1.length(), "!,"));
        }
        if (text.find(value3) < text.length()) {
            copyText = ((copyText).replace(text.find(value3), value1.length(), "!;"));
        }

        return copyText;
    }

    void GCanvasContext2D::setContentDirty() {
        _gCanvasNode->setDirty(true);
    }

    void GCanvasContext2D::scheduleFlushIfNeed() {
        if (_gCanvasNode->dirty()) {
            _renderCommandLine += _renderCommandFragment;
            _renderCommandFragment.clear();
            _gCanvasNode->scheduleUpdate();
        }
    }
}