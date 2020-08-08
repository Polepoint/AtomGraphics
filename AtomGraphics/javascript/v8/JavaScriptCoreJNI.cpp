#include <jni.h>
#include "thread/GraphicsThread.h"
#include "base/AtomLog.h"
#include "android/AtomGraphicsJNIHelper.h"
#include "graphics/android/DisplayLinkAndroid.h"
#include "V8JavaScriptInterface.h"

using namespace AtomGraphics;
using namespace AtomGraphics::JS;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_GraphicsJavaScriptCore_nativeCreateJavaScriptInterface(JNIEnv *env,
                                                                                 jobject instance) {
    if (!AtomGraphicsBase::IsVMInitialized()) {
        JavaVM *javaVM;
        env->GetJavaVM(&javaVM);
        AtomGraphicsBase::InitVM(javaVM);
    }

    V8JavaScriptInterface *interface = new V8JavaScriptInterface(env->NewGlobalRef(instance));
    return reinterpret_cast<intptr_t>(interface);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_GraphicsJavaScriptCore_nativeLoadJavaScript(JNIEnv *env,
                                                                      jobject instance,
                                                                      jlong nativeJavaScriptInterface,
                                                                      jstring script_) {
    const char *script = env->GetStringUTFChars(script_, 0);

    V8JavaScriptInterface *interface = reinterpret_cast<V8JavaScriptInterface *>(nativeJavaScriptInterface);
    char *scriptCopy = static_cast<char *>(malloc((strlen(script) + 1) * sizeof(char)));
    strcpy(scriptCopy, script);
    GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([scriptCopy, interface]() {
        v8::HandleScope scope(interface->getIsolate());
        interface->evaluateScript(scriptCopy);
        free(scriptCopy);
    });

    env->ReleaseStringUTFChars(script_, script);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_GraphicsJavaScriptCore_nativeReset(JNIEnv *env, jobject instance,
                                                             jlong nativeJavaScriptInterface) {
    V8JavaScriptInterface *interface = reinterpret_cast<V8JavaScriptInterface *>(nativeJavaScriptInterface);
    GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([interface] {
        interface->reset();
    });
}


}
