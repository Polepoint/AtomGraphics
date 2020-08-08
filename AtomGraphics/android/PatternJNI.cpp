//
// Created by neo on 2019-09-28.
//

#include <jni.h>
#include "node/canvas/CanvasNode.h"
#include "node/canvas/CanvasImageSource.h"
#include "PatternJNI.h"


using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_canvas_Pattern_nativeConstructor(JNIEnv *env, jobject instance, jlong nodePtr,
                                                                jboolean repeatX, jboolean repeatY) {

    auto *node = reinterpret_cast<CanvasNode *>(nodePtr);
    CanvasImageSource imageSource(node);
    auto *patternJNI = new AtomGraphics::PatternJNI(
            std::make_shared<AtomGraphics::Pattern>(&imageSource, repeatX, repeatY));

    return reinterpret_cast<jlong>(patternJNI);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_Pattern_nativeDestructor(JNIEnv *env, jobject instance, jlong ptr) {

    auto *patternJNI = reinterpret_cast<AtomGraphics::PatternJNI *>(ptr);
    delete patternJNI;
}


}