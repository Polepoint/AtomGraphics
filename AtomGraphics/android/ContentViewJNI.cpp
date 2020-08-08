//
// Created by neo on 2019/1/8.
//

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "ContentView.h"
#include "ContentViewUpdateDispatcher.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_ContentView_nativeInit(JNIEnv *env, jobject instance) {
    auto cInstance = new ContentView(env->NewGlobalRef(instance));
    ContentViewUpdateDispatcher::SharedInstance()->registerContentView(cInstance);
    return reinterpret_cast<intptr_t>(cInstance);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_ContentView_nativeDestroy(JNIEnv *env, jobject instance, jlong nativePtr) {
    auto *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return;
    }
    ContentViewUpdateDispatcher::SharedInstance()->unregisterContentView(cInstance);
    ContentViewUpdateDispatcher::SharedInstance()->contentViewStopRendering(cInstance);
    cInstance->destroy();
}

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_ContentView_nativeGetPageID(JNIEnv *env, jobject instance, jlong nativePtr) {
    auto *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return -1;
    }
    jlong pageID = cInstance->pageID();
    return pageID;
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_ContentView_nativeSurfaceCreated(JNIEnv *env, jobject instance, jlong nativePtr,
                                                                 jobject surface, jint width, jint height) {
    auto *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return;
    }
    cInstance->surfaceCreated(surface, width, height);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_ContentView_nativeSurfaceDestroyed(JNIEnv *env, jobject instance, jlong nativePtr) {
    auto *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return;
    }
    cInstance->surfaceDestroyed();
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_ContentView_nativeSurfaceChanged(JNIEnv *env, jobject instance, jlong nativePtr,
                                                                 jobject surface, jint width, jint height) {
    ContentView *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return;
    }
    cInstance->surfaceChanged(surface, width, height);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_ContentView_nativeUpdateVisibleRect(JNIEnv *env, jobject instance,
                                                                    jlong nativePtr,
                                                                    jint x, jint y, jint width, jint height) {
    auto *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return;
    }
    cInstance->updateVisibleBounds(x, y, width, height);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_ContentView_nativeSetViewVisibility(JNIEnv *env, jobject instance, jlong nativePtr,
                                                                    jboolean visible) {

    auto *cInstance = reinterpret_cast<ContentView *>(nativePtr);
    if (!cInstance) {
        return;
    }
    cInstance->updateVisibility(visible);

}

};