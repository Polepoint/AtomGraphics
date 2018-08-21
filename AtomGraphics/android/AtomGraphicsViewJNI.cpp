//
// Created by neo on 2018/8/7.
//
#include <jni.h>
#include <thread/GraphicsThread.h>

using namespace AtomGraphics;

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_AtomGraphicsView_initGraphicsEnv(JNIEnv *env, jobject instance) {

    // TODO
    GraphicsThread::InitMainThread();
}

}
