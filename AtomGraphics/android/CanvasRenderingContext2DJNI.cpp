//
// Created by neo on 2019-09-28.
//


#include <jni.h>
#include "node/canvas/CanvasRenderingContext2D.h"
#include "CanvasRenderingContext2DJNI.h"
#include "ImageDataJNI.h"
#include "PatternJNI.h"
#include "GradientJNI.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeDestructor(JNIEnv *env, jobject instance,
                                                                                jlong ptr) {
    delete reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr);
}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetFillStyleColor(JNIEnv *env, jobject instance,
                                                                                       jlong ptr, jfloat r, jfloat g,
                                                                                       jfloat b, jfloat a) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setFillStyle(CanvasStyle(Color4F(r, g, b, a)));
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetFillStylePattern(JNIEnv *env, jobject instance,
                                                                                         jlong ptr, jlong patternPtr) {

    auto *patternJNI = reinterpret_cast<PatternJNI *>(patternPtr);
    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setFillStyle(CanvasStyle(patternJNI->getPattern()));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetFillStyleGradient(JNIEnv *env, jobject instance,
                                                                                          jlong ptr,
                                                                                          jlong gradientPtr) {

    auto *gradient = reinterpret_cast<GradientJNI *>(gradientPtr);
    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setFillStyle(CanvasStyle(gradient->getGradient()));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetStrokeStyleColor(JNIEnv *env, jobject instance,
                                                                                         jlong ptr, jfloat r, jfloat g,
                                                                                         jfloat b, jfloat a) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setStrokeStyle(CanvasStyle(Color4F(r, g, b, a)));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetStrokeStylePattern(JNIEnv *env,
                                                                                           jobject instance, jlong ptr,
                                                                                           jlong patternPtr) {

    auto *patternJNI = reinterpret_cast<PatternJNI *>(patternPtr);
    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setStrokeStyle(CanvasStyle(patternJNI->getPattern()));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetStrokeStyleGradient(JNIEnv *env,
                                                                                            jobject instance, jlong ptr,
                                                                                            jlong gradientPtr) {

    auto *gradient = reinterpret_cast<GradientJNI *>(gradientPtr);
    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setStrokeStyle(CanvasStyle(gradient->getGradient()));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetShadowColor(JNIEnv *env, jobject instance,
                                                                                    jlong ptr, jfloat r, jfloat g,
                                                                                    jfloat b, jfloat a) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setShadowColor(Color4F(r, g, b, a));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetShadowBlur(JNIEnv *env, jobject instance,
                                                                                   jlong ptr, jfloat blur) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setShadowBlur(blur);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetShadowOffset(JNIEnv *env, jobject instance,
                                                                                     jlong ptr, jfloat offsetX,
                                                                                     jfloat offsetY) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setShadowOffsetX(offsetX);
    context->setShadowOffsetY(offsetY);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetLineCap(JNIEnv *env, jobject instance,
                                                                                jlong ptr, jint cap) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setLineCap(static_cast<const LineCap>(cap));

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetLineJoin(JNIEnv *env, jobject instance,
                                                                                 jlong ptr, jint join) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setLineJoin(static_cast<const LineJoin>(join));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetLineDash(JNIEnv *env, jobject instance,
                                                                                 jlong ptr, jobjectArray dashArray) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetLineDashOffset(JNIEnv *env, jobject instance,
                                                                                       jlong ptr, jfloat dashOffset) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setLineDashOffset(dashOffset);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_neativeSetLineWidth(JNIEnv *env, jobject instance,
                                                                                   jlong ptr, jfloat width) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setLineWidth(width);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetMiterLimit(JNIEnv *env, jobject instance,
                                                                                   jlong ptr, jfloat limit) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setMiterLimit(limit);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeRect(JNIEnv *env, jobject instance, jlong ptr,
                                                                          jfloat x, jfloat y, jfloat width,
                                                                          jfloat height) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->rect(x, y, width, height);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeFillRect(JNIEnv *env, jobject instance, jlong ptr,
                                                                              jfloat x, jfloat y, jfloat width,
                                                                              jfloat height) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->fillRect(x, y, width, height);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeStrokeRect(JNIEnv *env, jobject instance,
                                                                                jlong ptr, jfloat x, jfloat y,
                                                                                jfloat width, jfloat height) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->strokeRect(x, y, width, height);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeClearRect(JNIEnv *env, jobject instance, jlong ptr,
                                                                               jfloat x, jfloat y, jfloat width,
                                                                               jfloat height) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->clearRect(x, y, width, height);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeFill(JNIEnv *env, jobject instance, jlong ptr,
                                                                          jint windRule) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->fill(static_cast<WindRule>(windRule));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeStroke(JNIEnv *env, jobject instance, jlong ptr) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->stroke();

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeBeginPath(JNIEnv *env, jobject instance,
                                                                               jlong ptr) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->beginPath();

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeMoveTo(JNIEnv *env, jobject instance, jlong ptr,
                                                                            jfloat x, jfloat y) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->moveTo(x, y);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeClosePath(JNIEnv *env, jobject instance,
                                                                               jlong ptr) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->closePath();

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeLineTo(JNIEnv *env, jobject instance, jlong ptr,
                                                                            jfloat x, jfloat y) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->lineTo(x, y);

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeClip(JNIEnv *env, jobject instance, jlong ptr,
                                                                          jint windRule) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->clip(static_cast<WindRule>(windRule));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeQuadraticCurveTo(JNIEnv *env, jobject instance,
                                                                                      jlong ptr, jfloat cpx, jfloat cpy,
                                                                                      jfloat x, jfloat y) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->quadraticCurveTo(cpx, cpy, x, y);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeBezierCurveTo(JNIEnv *env, jobject instance,
                                                                                   jlong ptr, jfloat cp1x, jfloat cp1y,
                                                                                   jfloat cp2x, jfloat cp2y, jfloat x,
                                                                                   jfloat y) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeArc(JNIEnv *env, jobject instance, jlong ptr,
                                                                         jfloat x, jfloat y, jfloat r, jfloat sAngle,
                                                                         jfloat eAngle, jboolean counterclockwise) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->arc(x, y, r, sAngle, eAngle, counterclockwise);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeArcTo(JNIEnv *env, jobject instance, jlong ptr,
                                                                           jfloat x1, jfloat y1, jfloat x2, jfloat y2,
                                                                           jfloat r) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->arcTo(x1, y1, x2, y2, r);

}


JNIEXPORT jboolean JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeIsPointInPath(JNIEnv *env, jobject instance,
                                                                                   jlong ptr, jfloat x, jfloat y) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    return static_cast<jboolean>(context->isPointInPath(x, y));
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeScale(JNIEnv *env, jobject instance, jlong ptr,
                                                                           jfloat sx, jfloat sy) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->scale(sx, sy);

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeRotate(JNIEnv *env, jobject instance, jlong ptr,
                                                                            jfloat angleInRadians) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->rotate(angleInRadians);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeTranslate(JNIEnv *env, jobject instance, jlong ptr,
                                                                               jfloat tx, jfloat ty) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->translate(tx, tx);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeTransform(JNIEnv *env, jobject instance, jlong ptr,
                                                                               jfloat a, jfloat b, jfloat c, jfloat d,
                                                                               jfloat tx, jfloat ty) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->transform(a, b, c, d, tx, ty);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetTransform(JNIEnv *env, jobject instance,
                                                                                  jlong ptr, jfloat a, jfloat b,
                                                                                  jfloat c, jfloat d, jfloat tx,
                                                                                  jfloat ty) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setTransform(a, b, c, d, tx, ty);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetFont(JNIEnv *env, jobject instance, jlong ptr,
                                                                             jstring fontCSSString_) {
    const char *fontCSSString = env->GetStringUTFChars(fontCSSString_, 0);

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setFont(fontCSSString);

    env->ReleaseStringUTFChars(fontCSSString_, fontCSSString);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetTextAlign(JNIEnv *env, jobject instance,
                                                                                  jlong ptr, jint textAlign) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setTextAlign(static_cast<TextAlign>(textAlign));

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetTextBaseline(JNIEnv *env, jobject instance,
                                                                                     jlong ptr, jint textBaseline) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setTextBaseline(static_cast<TextBaseline>(textBaseline));

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeFillText(JNIEnv *env, jobject instance, jlong ptr,
                                                                              jstring text_, jfloat x, jfloat y,
                                                                              jboolean hasMaxWidth, jfloat maxWidth) {
    const char *text = env->GetStringUTFChars(text_, 0);

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    float max_width = maxWidth;
    context->fillText(base::String(text, strlen(text)), x, y, hasMaxWidth ? &max_width : nullptr);

    env->ReleaseStringUTFChars(text_, text);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeStrokeText(JNIEnv *env, jobject instance,
                                                                                jlong ptr, jstring text_, jfloat x,
                                                                                jfloat y, jboolean hasMaxWidth,
                                                                                jfloat maxWidth) {
    const char *text = env->GetStringUTFChars(text_, 0);

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    float max_width = maxWidth;
    context->strokeText(base::String(text, strlen(text)), x, y, hasMaxWidth ? &max_width : nullptr);

    env->ReleaseStringUTFChars(text_, text);
}


JNIEXPORT jfloat JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeMeasureText(JNIEnv *env, jobject instance,
                                                                                 jlong ptr, jstring text_) {
    const char *text = env->GetStringUTFChars(text_, 0);

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    TextMetrics metrics = context->measureText(base::String(text, strlen(text)));

    env->ReleaseStringUTFChars(text_, text);

    return metrics.getWidth();
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeDrawImage(JNIEnv *env, jobject instance, jlong ptr,
                                                                               jlong nodePtr, jfloat srcX,
                                                                               jfloat srcY, jfloat srcWidth,
                                                                               jfloat srcHeight,
                                                                               jfloat dstX, jfloat dstY,
                                                                               jfloat dstWidth,
                                                                               jfloat dstHeight) {

    CanvasImageSource source(reinterpret_cast<CanvasNode *>(nodePtr));
    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->drawImage(&source, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight);

}

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeGetImageData(JNIEnv *env, jobject instance,
                                                                                  jlong ptr, jint x, jint y, jint width,
                                                                                  jint height) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    ImageDataJNI *imageDataJNI = new ImageDataJNI(std::move(context->getImageData(x, y, width, height)));

    return reinterpret_cast<jlong>(imageDataJNI);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativePutImageData(JNIEnv *env,
                                                                                  jobject instance, jlong ptr,
                                                                                  jlong imageDataPtr, jint x,
                                                                                  jint y, jint dirtyX,
                                                                                  jint dirtyY,
                                                                                  jint dirtyWidth,
                                                                                  jint dirtyHeight) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->putImageData(reinterpret_cast<ImageData *>(imageDataPtr), x, y, dirtyX, dirtyY, dirtyWidth, dirtyHeight);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetGlobalAlpha(JNIEnv *env, jobject instance,
                                                                                    jlong ptr, jfloat alpha) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setGlobalAlpha(alpha);

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSetGlobalCompositeOperation(JNIEnv *env,
                                                                                                 jobject instance,
                                                                                                 jlong ptr,
                                                                                                 jint operation,
                                                                                                 jint blendMode) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->setGlobalCompositeOperation(static_cast<const CompositeOperator>(operation),
                                         static_cast<const BlendMode>(blendMode));

}


JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeSave(JNIEnv *env, jobject instance, jlong ptr) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->save();

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasRenderingContext2D_nativeRestore(JNIEnv *env, jobject instance, jlong ptr) {

    auto *context = reinterpret_cast<CanvasRenderingContext2DJNI *>(ptr)->getContext();
    context->stroke();

}

}

