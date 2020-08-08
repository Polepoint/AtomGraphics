//
// Created by neo on 2019-09-28.
//

#include <jni.h>
#include "node/canvas/android/skia/CanvasNodeSkia.h"
#include "node/canvas/CanvasNode.h"
#include "CanvasRenderingContext2DJNI.h"
#include "CanvasNodeJNI.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasNode_nativeConstructor(JNIEnv *env, jobject instance) {

    auto *nodeSkia = new CanvasNodeSkia();
    return reinterpret_cast<jlong>(nodeSkia);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasNode_nativeDestructor(JNIEnv *env, jobject instance, jlong ptr) {

    auto *node = reinterpret_cast<CanvasNodeJNI *>(ptr);
    delete node;

}


JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_canvas_CanvasNode_nativeGetCanvasContext(JNIEnv *env, jobject instance, jlong ptr) {

    auto *node = reinterpret_cast<CanvasNodeJNI *>(ptr);
    std::shared_ptr<CanvasRenderingContext2D> context = node->getNode()->getContext2d();
    return reinterpret_cast<jlong>(new CanvasRenderingContext2DJNI(context));

}


}