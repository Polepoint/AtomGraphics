//
// Created by neo on 2018/5/23.
//

#include "AtomGraphicsJNIHelper.h"
#include <sys/prctl.h>

namespace AtomGraphicsBase {

    static JavaVM *_jvm = nullptr;

    static jobject global_context;

    JNIEnv *AttachCurrentThread() {
        if (!_jvm) {
            return nullptr;
        }

        JNIEnv *env = nullptr;
        jint ret = _jvm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_2);
        if (ret == JNI_EDETACHED || !env) {
            JavaVMAttachArgs args;
            args.version = JNI_VERSION_1_2;
            args.group = nullptr;

            // 16 is the maximum size for thread names on Android.
            char thread_name[16];
            int err = prctl(PR_GET_NAME, thread_name);
            if (err < 0) {
                args.name = nullptr;
            } else {
                args.name = thread_name;
            }

            ret = _jvm->AttachCurrentThread(&env, &args);
            if (ret != JNI_OK) {
                return nullptr;
            }
        }
        return env;
    }

    void InitVM(JavaVM *vm) {
        _jvm = vm;
    }

    void DetachFromVM() {

    }

    bool IsVMInitialized() {
        return _jvm != nullptr;
    }

    void InitApplicationContext(JNIEnv *env, jobject context) {
        global_context = env->NewGlobalRef(context);
    }

    jclass FindClass(JNIEnv *jni, const char *class_name) {
        jclass context_class = jni->GetObjectClass(global_context);
        jmethodID get_class_loader = jni->GetMethodID(context_class, "getClassLoader",
                                                      "()Ljava/lang/ClassLoader;");
        jobject classloader_obj = jni->CallObjectMethod(global_context, get_class_loader);
        jclass class_loader = jni->FindClass("java/lang/ClassLoader");
        jmethodID find_class = jni->GetMethodID(class_loader, "loadClass",
                                                "(Ljava/lang/String;)Ljava/lang/Class;");

        jstring str_class_name = jni->NewStringUTF(class_name);
        jclass class_retrieved = (jclass) jni->CallObjectMethod(classloader_obj, find_class,
                                                                str_class_name);
        jni->DeleteLocalRef(str_class_name);
        jni->DeleteLocalRef(context_class);
        jni->DeleteLocalRef(class_loader);
        return class_retrieved;
    }

    extern "C" {

    jint JNI_OnLoad(JavaVM *vm, void *reserved) {
        AtomGraphicsBase::InitVM(vm);

        return JNI_VERSION_1_6;
    }

    JNIEXPORT void JNICALL
    Java_com_neo_atomgraphics_base_ContextUtils_initNativeContext(JNIEnv *env, jclass type,
                                                                  jobject context) {
        global_context = env->NewGlobalRef(context);
    }

    }
}

