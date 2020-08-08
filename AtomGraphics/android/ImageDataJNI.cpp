//
// Created by neo on 2019-09-28.
//

#include <jni.h>
#include "ImageDataJNI.h"

namespace AtomGraphics {

ImageDataJNI::ImageDataJNI(scoped_refptr<AtomGraphics::ImageData> imageData)
        : m_imageData(std::move(imageData)) {

}

}

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_canvas_ImageData_nativeDestructor(JNIEnv *env, jobject instance, jlong ptr) {

    auto *imageDataJNI = reinterpret_cast<AtomGraphics::ImageDataJNI *>(ptr);
    delete imageDataJNI;
}

JNIEXPORT jint JNICALL
Java_com_neo_atomgraphics_node_canvas_ImageData_nativeGetWidth(JNIEnv *env, jobject instance, jlong ptr) {

    auto *imageDataJNI = reinterpret_cast<AtomGraphics::ImageDataJNI *>(ptr);
    return imageDataJNI->getImageData()->size().width;

}


JNIEXPORT jint JNICALL
Java_com_neo_atomgraphics_node_canvas_ImageData_nativeGetHeight(JNIEnv *env, jobject instance, jlong ptr) {

    auto *imageDataJNI = reinterpret_cast<AtomGraphics::ImageDataJNI *>(ptr);
    return imageDataJNI->getImageData()->size().height;

}

}