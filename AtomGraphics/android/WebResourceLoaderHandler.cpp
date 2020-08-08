//
// Created by neo on 2020/5/3.
//


#include "WebResourceLoaderHandler.h"
#include "AtomGraphicsJNIHelper.h"
#include <android/bitmap.h>

extern "C" {

using namespace AtomGraphics;

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_io_WebResourceLoader_onImageLoaded(JNIEnv *env,
                                                                   jclass clazz,
                                                                   jlong callback_ptr,
                                                                   jobject bitmap) {

    WebResourceLoaderHandler *loaderHandler = reinterpret_cast<WebResourceLoaderHandler *>(callback_ptr);

    AndroidBitmapInfo info;
    bool failed = false;
    if (AndroidBitmap_getInfo(env, bitmap, &info) == ANDROID_BITMAP_RESULT_SUCCESS) {
        void *addrPtr;
        if (AndroidBitmap_lockPixels(env, bitmap, &addrPtr) == ANDROID_BITMAP_RESULT_SUCCESS) {
            loaderHandler->complete(addrPtr, info.stride * info.height, info.width, info.height);
            AndroidBitmap_unlockPixels(env, bitmap);
        } else {
            failed = true;
        }
    } else {
        failed = true;
    }

    if (failed) {
        loaderHandler->complete(nullptr, 0, 0, 0);
    }

    delete loaderHandler;
}

}

namespace AtomGraphics {

void WebResourceLoaderHandler::loadResource(
        const std::string &url,
        const std::function<void(void *, uint32_t, uint32_t, uint32_t)> &completeCallback) {

    JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
    jclass clazz = AtomGraphicsBase::FindClass(
            env, "com/neo/atomgraphics/io/WebResourceLoader");

    jmethodID method_id = env->GetStaticMethodID(clazz, "loadImage", "(Ljava/lang/String;J)V");


    jstring urlStr = env->NewStringUTF(url.c_str());
    jlong ptr = reinterpret_cast<intptr_t>(new WebResourceLoaderHandler(completeCallback));
    env->CallStaticVoidMethod(clazz, method_id, urlStr, ptr);

    env->DeleteLocalRef(urlStr);
    env->DeleteLocalRef(clazz);
}

void WebResourceLoaderHandler::complete(void *bytes,
                                        uint32_t bufferSize,
                                        uint32_t width,
                                        uint32_t height) {
    m_completeCallback(bytes, bufferSize, width, height);
}

}
