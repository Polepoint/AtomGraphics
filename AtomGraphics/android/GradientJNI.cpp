//
// Created by neo on 2019-09-28.
//

#include <jni.h>
#include "GradientJNI.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_canvas_Gradient_nativeConstructor__FFFF(JNIEnv *env, jobject instance, jfloat x0,
                                                                       jfloat y0, jfloat x1, jfloat y1) {
    auto canvas_gradient = std::make_shared<Gradient>(x0, y0, x1, y1);
    return reinterpret_cast<jlong>(new GradientJNI(std::move(canvas_gradient)));
}

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_canvas_Gradient_nativeConstructor__FFFFFF(JNIEnv *env, jobject instance, jfloat x0,
                                                                         jfloat y0, jfloat r0, jfloat x1, jfloat y1,
                                                                         jfloat r1) {

    auto canvas_gradient = std::make_shared<Gradient>(x0, y0, r0, x1, y1, r1);
    return reinterpret_cast<jlong>(new GradientJNI(std::move(canvas_gradient)));
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_Gradient_nativeDestructor(JNIEnv *env, jobject instance, jlong ptr) {

    auto *gradientJNI = reinterpret_cast<GradientJNI *>(ptr);
    delete gradientJNI;
}

}