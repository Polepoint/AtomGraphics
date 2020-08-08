package com.neo.atomgraphics.node.canvas;

import android.graphics.Color;
import android.graphics.Point;

import java.util.ArrayList;

public class CanvasRenderingContext2D {

    private long mPtr;

    CanvasRenderingContext2D(long ptr) {
        this.mPtr = ptr;
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeDestructor(mPtr);
    }

    private native void nativeDestructor(long ptr);

    public void setFillStyle(int color) {
        nativeSetFillStyleColor(mPtr, Color.red(color) / 255.0f, Color.green(color) / 255.0f, Color.blue(color) / 255.0f, Color.alpha(color) / 255.0f);
    }

    private native void nativeSetFillStyleColor(long ptr, float r, float g, float b, float a);

    public void setFillStyle(Pattern pattern) {
        nativeSetFillStylePattern(mPtr, pattern.nativePtr());
    }

    private native void nativeSetFillStylePattern(long ptr, long patternPtr);

    public void setFillStyle(Gradient gradient) {
        nativeSetFillStyleGradient(mPtr, gradient.nativePtr());
    }

    private native void nativeSetFillStyleGradient(long ptr, long gradientPtr);

    public void setStrokeStyle(int color) {
        nativeSetStrokeStyleColor(mPtr, Color.red(color) / 255.0f, Color.green(color) / 255.0f, Color.blue(color) / 255.0f, Color.alpha(color) / 255.0f);
    }

    private native void nativeSetStrokeStyleColor(long ptr, float r, float g, float b, float a);

    public void setStrokeStyle(Pattern pattern) {
        nativeSetStrokeStylePattern(mPtr, pattern.nativePtr());
    }

    private native void nativeSetStrokeStylePattern(long ptr, long patternPtr);

    public void setStrokeStyle(Gradient gradient) {
        nativeSetStrokeStyleGradient(mPtr, gradient.nativePtr());
    }

    private native void nativeSetStrokeStyleGradient(long ptr, long gradientPtr);

    public void setShadowColor(int color) {
        nativeSetShadowColor(mPtr, Color.red(color) / 255.0f, Color.green(color) / 255.0f, Color.blue(color) / 255.0f, Color.alpha(color) / 255.0f);
    }

    private native void nativeSetShadowColor(long ptr, float r, float g, float b, float a);

    public void setShadowBlur(float blur) {
        nativeSetShadowBlur(mPtr, blur);
    }

    private native void nativeSetShadowBlur(long ptr, float blur);

    public void setShadowOffset(float offsetX, float offsetY) {
        nativeSetShadowOffset(mPtr, offsetX, offsetY);
    }

    private native void nativeSetShadowOffset(long ptr, float offsetX, float offsetY);

    public void setLineCap(CanvasType.LineCap cap) {
        nativeSetLineCap(mPtr, cap.ordinal());
    }

    private native void nativeSetLineCap(long ptr, int cap);

    public void setLineJoin(CanvasType.LineJoin lineJoin) {
        nativeSetLineJoin(mPtr, lineJoin.ordinal());
    }

    private native void nativeSetLineJoin(long ptr, int join);

    public void setLineDash(ArrayList<Float> dashArray) {
        nativeSetLineDash(mPtr, dashArray.toArray());
    }

    private native void nativeSetLineDash(long ptr, Object[] dashArray);

    public void setLineDashOffset(float dashOffset) {
        nativeSetLineDashOffset(mPtr, dashOffset);
    }

    private native void nativeSetLineDashOffset(long ptr, float dashOffset);

    public void setLineWidth(float width) {
        nativeSetLineDashOffset(mPtr, width);
    }

    private native void neativeSetLineWidth(long ptr, float width);

    public void setMiterLimit(float limit) {
        nativeSetMiterLimit(mPtr, limit);
    }

    private native void nativeSetMiterLimit(long ptr, float limit);

    public void rect(float x, float y, float width, float height) {
        nativeRect(mPtr, x, y, width, height);
    }

    private native void nativeRect(long ptr, float x, float y, float width, float height);

    public void fillRect(float x, float y, float width, float height) {
        nativeFillRect(mPtr, x, y, width, height);
    }

    private native void nativeFillRect(long ptr, float x, float y, float width, float height);

    public void strokeRect(float x, float y, float width, float height) {
        nativeStrokeRect(mPtr, x, y, width, height);
    }

    private native void nativeStrokeRect(long ptr, float x, float y, float width, float height);

    public void clearRect(float x, float y, float width, float height) {
        nativeClearRect(mPtr, x, y, width, height);
    }

    private native void nativeClearRect(long ptr, float x, float y, float width, float height);

    public void fill() {
        fill(CanvasType.WindRule.NonZero);
    }

    public void fill(CanvasType.WindRule windRule) {
        nativeFill(mPtr, windRule.ordinal());
    }

    private native void nativeFill(long ptr, int windRule);

    public void stroke() {
        nativeStroke(mPtr);
    }

    private native void nativeStroke(long ptr);

    public void beginPath() {
        nativeBeginPath(mPtr);
    }

    private native void nativeBeginPath(long ptr);

    public void moveTo(float x, float y) {
        nativeMoveTo(mPtr, x, y);
    }

    private native void nativeMoveTo(long ptr, float x, float y);

    public void closePath() {
        nativeClosePath(mPtr);
    }

    private native void nativeClosePath(long ptr);

    public void lineTo(float x, float y) {
        nativeLineTo(mPtr, x, y);
    }

    private native void nativeLineTo(long ptr, float x, float y);

    public void clip() {
        clip(CanvasType.WindRule.NonZero);
    }

    public void clip(CanvasType.WindRule windRule) {
        nativeClip(mPtr, windRule.ordinal());
    }

    private native void nativeClip(long ptr, int windRule);

    public void quadraticCurveTo(float cpx, float cpy, float x, float y) {
        nativeQuadraticCurveTo(mPtr, cpx, cpy, x, y);
    }

    private native void nativeQuadraticCurveTo(long ptr, float cpx, float cpy, float x, float y);

    public void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
        nativeBezierCurveTo(mPtr, cp1x, cp1y, cp2x, cp2y, x, y);
    }

    private native void nativeBezierCurveTo(long ptr, float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);

    public void arc(float x, float y, float r, float sAngle, float eAngle, boolean counterclockwise) {
        nativeArc(mPtr, x, y, r, sAngle, eAngle, counterclockwise);
    }

    private native void nativeArc(long ptr, float x, float y, float r, float sAngle, float eAngle, boolean counterclockwise);

    public void arcTo(float x1, float y1, float x2, float y2, float r) {
        nativeArcTo(mPtr, x1, y1, x2, y2, r);
    }

    private native void nativeArcTo(long ptr, float x1, float y1, float x2, float y2, float r);

    boolean isPointInPath(float x, float y) {
        return nativeIsPointInPath(mPtr, x, y);
    }

    private native boolean nativeIsPointInPath(long ptr, float x, float y);

    public void scale(float sx, float sy) {
        nativeScale(mPtr, sx, sy);
    }

    private native void nativeScale(long ptr, float sx, float sy);

    public void rotate(float angleInRadians) {
        nativeRotate(mPtr, angleInRadians);
    }

    private native void nativeRotate(long ptr, float angleInRadians);


    public void translate(float tx, float ty) {
        nativeTranslate(mPtr, tx, ty);
    }

    private native void nativeTranslate(long ptr, float tx, float ty);

    public void transform(float a, float b, float c, float d, float tx, float ty) {
        nativeTransform(mPtr, a, b, c, d, tx, ty);
    }

    private native void nativeTransform(long ptr, float a, float b, float c, float d, float tx, float ty);

    public void setTransform(float a, float b, float c, float d, float tx, float ty) {
        nativeSetTransform(mPtr, a, b, c, d, tx, ty);
    }

    private native void nativeSetTransform(long ptr, float a, float b, float c, float d, float tx, float ty);

    public void setFont(Font font) {
        nativeSetFont(mPtr, font.createCSSString());
    }

    private native void nativeSetFont(long ptr, String fontCSSString);

    public void setTextAlign(CanvasType.TextAlign textAlign) {
        nativeSetTextAlign(mPtr, textAlign.ordinal());
    }

    private native void nativeSetTextAlign(long ptr, int textAlign);


    public void setTextBaseline(CanvasType.TextBaseline textBaseline) {
        nativeSetTextBaseline(mPtr, textBaseline.ordinal());
    }

    private native void nativeSetTextBaseline(long ptr, int textBaseline);

    public void fillText(String text, float x, float y) {
        nativeFillText(mPtr, text, x, y, false, 0);
    }

    public void fillText(String text, float x, float y, float maxWidth) {
        nativeFillText(mPtr, text, x, y, true, maxWidth);
    }

    private native void nativeFillText(long ptr, String text, float x, float y, boolean hasMaxWidth, float maxWidth);

    public void strokeText(String text, float x, float y) {
        nativeStrokeText(mPtr, text, x, y, false, 0);
    }

    public void strokeText(String text, float x, float y, float maxWidth) {
        nativeStrokeText(mPtr, text, x, y, true, maxWidth);
    }

    private native void nativeStrokeText(long ptr, String text, float x, float y, boolean hasMaxWidth, float maxWidth);

    float measureText(String text) {
        return nativeMeasureText(mPtr, text);
    }

    private native float nativeMeasureText(long ptr, String text);

    public void drawImage(CanvasNode node, float x, float y) {
        Point size = node.getBoundsSize();
        drawImage(node, x, y, size.x, size.y);
    }

    public void drawImage(CanvasNode node, float x, float y, float width, float height) {
        Point size = node.getBoundsSize();
        drawImage(node, 0, 0, size.x, size.y, x, y, width, height);
    }

    public void drawImage(CanvasNode node,
                          float srcX, float srcY, float srcWidth, float srcHeight,
                          float dstX, float dstY, float dstWidth, float dstHeight) {
        nativeDrawImage(mPtr, node.getNativePtr(), srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight);
    }

    private native void nativeDrawImage(long ptr, long nodePtr, float srcX, float srcY, float srcWidth, float srcHeight,
                                        float dstX, float dstY, float dstWidth, float dstHeight);

    ImageData getImageData(int x, int y, int width, int height) {
        long imageDataPtr = nativeGetImageData(mPtr, x, y, width, height);
        return new ImageData(imageDataPtr);
    }

    private native long nativeGetImageData(long ptr, int x, int y, int width, int height);

    public void putImageData(ImageData imageData, int x, int y) {
        putImageData(imageData, x, y, 0, 0, imageData.getWidth(), imageData.getHeight());
    }

    public void putImageData(ImageData imageData, int x, int y, int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight) {
        nativePutImageData(mPtr, imageData.nativePtr(), x, y, dirtyX, dirtyY, dirtyWidth, dirtyHeight);
    }

    private native void nativePutImageData(long ptr, long imageDataPtr, int x, int y, int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight);

    public void setGlobalAlpha(float alpha) {
        nativeSetGlobalAlpha(mPtr, alpha);
    }

    private native void nativeSetGlobalAlpha(long ptr, float alpha);

    public void setGlobalCompositeOperation(CanvasType.CompositeOperator operation, CanvasType.BlendMode blendMode) {
        nativeSetGlobalCompositeOperation(mPtr, operation.ordinal(), blendMode.ordinal() + 1);
    }

    private native void nativeSetGlobalCompositeOperation(long ptr, int operation, int blendMode);

    public void save() {
        nativeSave(mPtr);
    }

    private native void nativeSave(long ptr);

    public void restore() {
        nativeRestore(mPtr);
    }

    private native void nativeRestore(long ptr);

}
