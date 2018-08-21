//
// Created by neo on 2018/7/30.
//
#include <jni.h>
#include <graphics/GraphicsPageContext.h>

using namespace AtomGraphics;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_graphics_GraphicsPageContext_createNativePageContext(JNIEnv *env, jobject instance,
                                                                               jobject jContext) {
    GraphicsPageContext *pageContext = new GraphicsPageContext(env->NewGlobalRef(jContext));
    return reinterpret_cast<intptr_t>(pageContext);
}

}