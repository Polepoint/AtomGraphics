//
// Created by neo on 2020/5/2.
//

#include <jni.h>
#include "android/AtomGraphicsJNIHelper.h"
#include "cache/DiskCache.h"

namespace AtomGraphics {

const char *DiskCache::kCacheFolderName = "atg_cache";

void DiskCache::checkCachePath() {
    if (!m_cachePathInitialized) {


        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz = AtomGraphicsBase::FindClass(
                env, "com/neo/atomgraphics/io/DiskCache");

        jmethodID method_id = env->GetStaticMethodID(clazz, "checkCachePath",
                                                     "(Ljava/lang/String;)Z");

        jstring folderName = env->NewStringUTF(DiskCache::kCacheFolderName);
        jboolean result = env->CallStaticBooleanMethod(clazz, method_id, folderName);

        env->DeleteLocalRef(folderName);
        env->DeleteLocalRef(clazz);

        if (!result) {
            // TODO: log error
        }

        m_cachePathInitialized = true;
    }
}

scoped_refptr<SharedBuffer> DiskCache::dataBufferFromFile(const char *fileName) {
    checkCachePath();

    scoped_refptr<SharedBuffer> buffer = MakeRefCounted<SharedBuffer>();

    JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
    jclass clazz = AtomGraphicsBase::FindClass(
            env, "com/neo/atomgraphics/io/DiskCache");

    jmethodID method_id = env->GetStaticMethodID(clazz, "readCachedFile",
                                                 "(Ljava/lang/String;J)V");

    jstring jFileName = env->NewStringUTF(fileName);
    jlong bufferPtr = reinterpret_cast<jlong>(buffer.get());
    env->CallStaticVoidMethod(clazz, method_id, jFileName, bufferPtr);

    if (!buffer->bufferSize()) {
        buffer.reset();
    }

    env->DeleteLocalRef(jFileName);
    env->DeleteLocalRef(clazz);
    return buffer;
}

void DiskCache::saveBufferDataToFile(const char *fileName, scoped_refptr<SharedBuffer> bufferData) {
    if (!bufferData || !bufferData->bufferSize()) {
        return;
    }

    checkCachePath();

    scoped_refptr<SharedBuffer> buffer = MakeRefCounted<SharedBuffer>();

    JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
    jclass clazz = AtomGraphicsBase::FindClass(
            env, "com/neo/atomgraphics/io/DiskCache");

    jmethodID method_id = env->GetStaticMethodID(clazz, "writeFile",
                                                 "(Ljava/lang/String;[B)V");

    jbyteArray jbyteArray = env->NewByteArray(bufferData->bufferSize());
    env->SetByteArrayRegion(jbyteArray,
                            0,
                            bufferData->bufferSize(),
                            static_cast<const jbyte *>(bufferData->data()));
    jstring jFileName = env->NewStringUTF(fileName);
    env->CallStaticVoidMethod(clazz, method_id, jFileName, jbyteArray);

    env->DeleteLocalRef(jbyteArray);
    env->DeleteLocalRef(jFileName);

}


}
