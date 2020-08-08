//
// Created by neo on 2019/3/14.
//


#include <jni.h>
#include "JSPageScope.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_javascript_JSPageScope_nativeInit(JNIEnv *env, jobject instance, jlong pageID,
                                                            jstring srcFileContent_, jlong javaScriptCorePtr) {
    const char *srcFileContent = env->GetStringUTFChars(srcFileContent_, 0);

    JS::V8JavaScriptInterface *v8JSInterface = reinterpret_cast<JS::V8JavaScriptInterface *>(javaScriptCorePtr);
    JS::JSPageScope *pageScope = new JS::JSPageScope(static_cast<long>(pageID), srcFileContent, v8JSInterface);

    env->ReleaseStringUTFChars(srcFileContent_, srcFileContent);

    return reinterpret_cast<intptr_t>(pageScope);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_javascript_JSPageScope_nativeDestroy(JNIEnv *env, jobject instance, jlong nativePtr,
                                                               jlong javaScriptCorePtr) {

    JS::JSPageScope *pageScope = reinterpret_cast<JS::JSPageScope *>(nativePtr);
    JS::V8JavaScriptInterface *v8JSInterface = reinterpret_cast<JS::V8JavaScriptInterface *>(javaScriptCorePtr);
    pageScope->destroy(v8JSInterface);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_javascript_JSPageScope_nativeEvaluateScript(JNIEnv *env,
                                                                      jobject instance,
                                                                      jlong nativePtr,
                                                                      jstring script_,
                                                                      jlong javaScriptCorePtr) {
    const char *script = env->GetStringUTFChars(script_, 0);

    JS::V8JavaScriptInterface *v8JSInterface = reinterpret_cast<JS::V8JavaScriptInterface *>(javaScriptCorePtr);
    JS::JSPageScope *pageScope = reinterpret_cast<JS::JSPageScope *>(nativePtr);
    pageScope->evaluateScript(script, v8JSInterface);

    env->ReleaseStringUTFChars(script_, script);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_javascript_JSPageScope_nativeCallJSHandler(JNIEnv *env,
                                                                     jobject instance,
                                                                     jlong nativePtr,
                                                                     jstring handlerName_,
                                                                     jstring data_,
                                                                     jlong javaScriptCorePtr) {
    const char *handlerName = env->GetStringUTFChars(handlerName_, 0);
    const char *data = nullptr;
    if (data_) {
        data = env->GetStringUTFChars(data_, 0);
    }

    JS::V8JavaScriptInterface *v8JSInterface = reinterpret_cast<JS::V8JavaScriptInterface *>(javaScriptCorePtr);
    JS::JSPageScope *pageScope = reinterpret_cast<JS::JSPageScope *>(nativePtr);
    pageScope->callJSHandler(handlerName, data, v8JSInterface);

    env->ReleaseStringUTFChars(handlerName_, handlerName);
    if (data) {
        env->ReleaseStringUTFChars(data_, data);
    }
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_javascript_JSPageScope_nativeRegisterNativeHandler(JNIEnv *env,
                                                                             jobject instance,
                                                                             jlong nativePtr,
                                                                             jstring handlerName_,
                                                                             jobject handler,
                                                                             jlong javaScriptCorePtr) {
    const char *handlerName = env->GetStringUTFChars(handlerName_, 0);

    JS::V8JavaScriptInterface *v8JSInterface = reinterpret_cast<JS::V8JavaScriptInterface *>(javaScriptCorePtr);
    JS::JSPageScope *pageScope = reinterpret_cast<JS::JSPageScope *>(nativePtr);
    pageScope->registerNativeHandler(handlerName, env->NewGlobalRef(handler), v8JSInterface);

    env->ReleaseStringUTFChars(handlerName_, handlerName);
}

}