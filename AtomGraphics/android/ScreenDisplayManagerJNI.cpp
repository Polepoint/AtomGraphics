//
// Created by neo on 2019/3/13.
//

#ifndef ATOMGRAPHICS_WINDOWDISPLAYMANAGERJNI_H
#define ATOMGRAPHICS_WINDOWDISPLAYMANAGERJNI_H


#include <jni.h>
#include "ScreenDisplayManager.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_display_ScreenDisplayManager_nativeUpdateDisplay(JNIEnv *env, jobject instance,
                                                                          jlong nativeScreenDisplayManager,
                                                                          jint sdkDisplayId, jint width,
                                                                          jint height, jfloat dipScale,
                                                                          jint bitsPerPixel, jint bitsPerComponent) {
    ScreenDisplayManager *manager = reinterpret_cast<ScreenDisplayManager *>(nativeScreenDisplayManager);
    manager->updateDisplay(sdkDisplayId, width, height, dipScale, bitsPerPixel, bitsPerComponent);

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_display_ScreenDisplayManager_nativeRemoveDisplay(JNIEnv *env, jobject instance,
                                                                          jlong nativeScreenDisplayManager,
                                                                          jint sdkDisplayId) {
    ScreenDisplayManager *manager = reinterpret_cast<ScreenDisplayManager *>(nativeScreenDisplayManager);
    return manager->removeDisplay(sdkDisplayId);
}

};


#endif //ATOMGRAPHICS_WINDOWDISPLAYMANAGERJNI_H
