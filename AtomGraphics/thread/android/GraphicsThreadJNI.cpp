//
// Created by neo on 2019/3/18.
//


#include <jni.h>
#include "android/AtomGraphicsJNIHelper.h"
#include "thread/GraphicsThread.h"


extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_thread_GraphicsThread_dispatch(JNIEnv *env, jclass type, jobject runnable_) {

    jobject runnable = env->NewGlobalRef(runnable_);
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue([runnable]() -> void {
        JNIEnv *jni = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz_runnable = jni->FindClass("java/lang/Runnable");
        jmethodID methodID = jni->GetMethodID(clazz_runnable, "run", "()V");
        jni->CallVoidMethod(runnable, methodID);
        jni->DeleteGlobalRef(runnable);
        jni->DeleteLocalRef(clazz_runnable);
    });
}

}

