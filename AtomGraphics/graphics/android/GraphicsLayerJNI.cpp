//
// Created by neo on 2018/7/29.
//

#include <jni.h>
#include "graphics/GraphicsLayerGCanvas.h"
#include "graphics/GraphicsLayer.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_graphics_GraphicsLayer_createNativeGraphicsLayer(JNIEnv *env,
                                                                           jobject instance,
                                                                           jlong platformLayerRef,
                                                                           jlong rootNodeRef,
                                                                           jstring layerType_) {
    const char *layerType = env->GetStringUTFChars(layerType_, 0);
    PlatformLayer *platformLayer = reinterpret_cast<PlatformLayer *>(platformLayerRef);
    Node *rootNode = reinterpret_cast<Node *>(rootNodeRef);
    GraphicsLayer *graphicsLayer = nullptr;
    if (strcmp(layerType, "default") == 0) {
        graphicsLayer = new GraphicsLayer(platformLayer, rootNode);;
    } else if (strcmp(layerType, "gcanvas") == 0) {
        graphicsLayer = new GraphicsLayerGCanvas(platformLayer, rootNode);
    }

    env->ReleaseStringUTFChars(layerType_, layerType);
    return reinterpret_cast<intptr_t>(graphicsLayer);
}

}
