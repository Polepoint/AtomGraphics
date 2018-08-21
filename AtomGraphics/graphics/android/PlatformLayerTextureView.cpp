//
// Created by neo on 2018/7/26.
//

#include <jni.h>
#include "android/AtomGraphicsJNIHelper.h"
#include "PlatformLayerTextureView.h"

namespace AtomGraphics {

    PlatformLayerTextureView::PlatformLayerTextureView(jobject textureViewObj)
            : m_textureViewObj(textureViewObj) {

    }

    void PlatformLayerTextureView::renderCommand(const char *command) {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz = env->GetObjectClass(m_textureViewObj);
        jmethodID method_id = env->GetMethodID(clazz, "renderCommand", "(Ljava/lang/String;)V");
        jstring commandLine = env->NewStringUTF(command);
        env->CallVoidMethod(m_textureViewObj, method_id, commandLine);
        env->DeleteLocalRef(commandLine);
    }
}

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_graphics_PlatformLayerTextureView_createNativePlatformLayerRef(
        JNIEnv *env, jobject instance, jobject textureView) {
    jobject jRef = env->NewGlobalRef(textureView);
    PlatformLayerTextureView *platformLayer = new PlatformLayerTextureView(jRef);
    return reinterpret_cast<intptr_t>(platformLayer);
}

}