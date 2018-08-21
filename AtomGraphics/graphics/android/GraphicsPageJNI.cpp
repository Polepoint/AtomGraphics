//
// Created by neo on 2018/7/29.
//

#include <jni.h>
#include <graphics/GraphicsPageManager.h>
#include "graphics/GraphicsPage.h"

using namespace AtomGraphics;

extern "C" {

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_graphics_GraphicsPage_release__J(JNIEnv *env, jobject instance,
                                                           jlong nativeGraphicsPageRef) {
    GraphicsPage *page = reinterpret_cast<GraphicsPage *>(nativeGraphicsPageRef);
    GraphicsPageManager::unregisterPage(page);
    page->release();
}

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_graphics_GraphicsPage_createNativeGraphicsPage(JNIEnv *env, jobject instance,
                                                                         jlong graphicsPageContextRef,
                                                                         jlong rootLayerRef) {
    GraphicsPageContext *pageContext = reinterpret_cast<GraphicsPageContext *>(graphicsPageContextRef);
    GraphicsLayer *layer = reinterpret_cast<GraphicsLayer *>(rootLayerRef);
    GraphicsPage *page = new GraphicsPage(pageContext, layer);
    return reinterpret_cast<intptr_t>(page);
}

}

