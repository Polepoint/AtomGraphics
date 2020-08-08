//
// Created by neo on 2020/5/3.
//

#include <jni.h>
#include "io/SharedBuffer.h"

using namespace AtomGraphics;

extern "C" {
JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_io_DiskCache_writeBufferData(JNIEnv *env,
                                                             jclass clazz,
                                                             jlong native_buffer,
                                                             jbyteArray buffer,
                                                             jlong buffer_size) {

    jbyte *bytes = env->GetByteArrayElements(buffer, 0);
    SharedBuffer *sharedBuffer = reinterpret_cast<SharedBuffer *>(native_buffer);
    sharedBuffer->copyData(bytes, static_cast<unsigned int>(buffer_size));

    env->ReleaseByteArrayElements(buffer, bytes, 0);
}

}

