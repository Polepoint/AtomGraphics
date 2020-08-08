//
// Created by neo on 2019/3/17.
//

#include <jni.h>
#include "thread/GraphicsThread.h"
#include "JSMessageHandlerCallback.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_javascript_JSMessageCallback_nativeCallback(JNIEnv *env,
                                                                      jobject instance,
                                                                      jlong nativePtr,
                                                                      jstring data_) {

    if (!GraphicsThread::IsGraphicsThread()) {
        return;
    }

    const char *data = env->GetStringUTFChars(data_, 0);

    JS::JSMessageHandlerCallback *callback = reinterpret_cast<JS::JSMessageHandlerCallback *>(nativePtr);
    callback->callback(data);

    env->ReleaseStringUTFChars(data_, data);
}

}