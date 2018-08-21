//
// Created by neo on 2018/5/23.
//

#ifndef ATOMGRAPHICS_ATOMGRAPHICSANDROIDJNI_H
#define ATOMGRAPHICS_ATOMGRAPHICSANDROIDJNI_H

#include <jni.h>

namespace AtomGraphicsBase {

    JNIEnv *AttachCurrentThread();

    void InitVM(JavaVM *vm);

    void DetachFromVM();

    bool IsVMInitialized();

    jclass FindClass(JNIEnv *jni, jobject context, const char *class_name);
}


#endif //ATOMGRAPHICS_ATOMGRAPHICSANDROIDJNI_H
