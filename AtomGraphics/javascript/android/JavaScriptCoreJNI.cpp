#include <jni.h>
#include "android/AtomGraphicsJNIHelper.h"
#include <android/asset_manager.h>
#include <graphics/android/DisplayLinkAndroid.h>
#include "JavaScriptInterface.h"

using namespace AtomGraphicsJSC;
using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_GraphicsJavaScriptCore_createNativeJavaScriptInterface(JNIEnv *env, jobject instance,
                                                                                 jlong pageContextRef) {
    if (!AtomGraphicsBase::IsVMInitialized()) {
        JavaVM *javaVM;
        env->GetJavaVM(&javaVM);
        AtomGraphicsBase::InitVM(javaVM);
    }

    GraphicsPageContext *pageContext = reinterpret_cast<GraphicsPageContext *>(pageContextRef);
    JavaScriptInterface *interface = new JavaScriptInterface(pageContext, env->NewGlobalRef(instance));
    return reinterpret_cast<intptr_t>(interface);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_GraphicsJavaScriptCore_loadJavaScript__JLjava_lang_String_2(JNIEnv *env, jobject instance,
                                                                                      jlong nativeJavaScriptInterface,
                                                                                      jstring script_) {
    const char *script = env->GetStringUTFChars(script_, 0);

    // TODO

    JavaScriptInterface *interface = reinterpret_cast<JavaScriptInterface *>(nativeJavaScriptInterface);
    interface->evaluateScript(script);
    env->ReleaseStringUTFChars(script_, script);
}

}
