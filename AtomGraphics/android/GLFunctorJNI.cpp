//
// Created by neo on 2019-08-15.
//


#include <jni.h>
#include "GLFunctor.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_GLFunctor_nativeCreate(JNIEnv *env, jobject instance, jlong contentViewPtr) {

    return reinterpret_cast<intptr_t>(new GLFunctor(env, instance, reinterpret_cast<ContentView *>(contentViewPtr)));
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_GLFunctor_nativeDestroy(JNIEnv *env, jobject instance, jlong nativePtr) {
    GLFunctor *functor = reinterpret_cast<GLFunctor *>(nativePtr);
    functor->destroy();
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_GLFunctor_nativeRemoveContentView(JNIEnv *env, jobject instance, jlong nativePtr) {

    GLFunctor *functor = reinterpret_cast<GLFunctor *>(nativePtr);
    functor->setContentView(nullptr);
}

}