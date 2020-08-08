//
// Created by neo on 2018/8/7.
//
#include <jni.h>
#include "thread/UIThread.h"
#include "base/android/DeviceAndroid.h"
#include "ScreenDisplayManager.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_GraphicsEnvironment_nativeInitGraphicsEnv(JNIEnv *env, jclass type) {

    UIThread::InitUIThread();
    DeviceAndroid::InitializePlatform();
}

}
