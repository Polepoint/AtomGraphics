//
// Created by neo on 2018/5/22.
//
#include <jni.h>
#include "DisplayLinkAndroid.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jboolean JNICALL
Java_com_neo_atomgraphics_DisplayLink_nativeOnVSync(JNIEnv *env, jobject instance,
                                                    jlong nativeDisplayLinkRef,
                                                    jlong vsyncTimeMicros) {

    DisplayLinkAndroid *displayLink = reinterpret_cast<DisplayLinkAndroid *>(nativeDisplayLinkRef);
    bool paused = displayLink->isPaused();
    if (!paused) {
        displayLink->displayLinkFired();
    }

    return static_cast<jboolean>(paused);
}

}
