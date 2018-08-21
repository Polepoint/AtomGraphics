//
// Created by neo on 2018/5/24.
//


#include <jni.h>
#include <string>
#include "node/AtomNode.h"
#include "node/canvas/AtomCanvasNode.h"
#include "node/canvas/AtomGCanvasNode.h"

using namespace AtomGraphics;

inline Node *CreateNode(JNIEnv *env, jobject instance, const std::string &type_name) {

    if (type_name.empty()) {
        return NULL;
    }

    if (type_name == "default") {
        return new Node();
    } else if (type_name == "canvas") {
        return new CanvasNode();
    } else if (type_name == "gcanvas") {
        GCanvasNode *node = new GCanvasNode();
        if (!node->getClassJObject()) {
            jclass nodeClass = env->GetObjectClass(instance);
            node->setClassJObject(static_cast<jclass>(env->NewGlobalRef(nodeClass)));
        }
        return node;
    }

    return nullptr;
}

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_node_Node_createNativeNode(JNIEnv *env, jobject instance,
                                                     jstring _type) {

    const char *type = env->GetStringUTFChars(_type, 0);
    Node *node = CreateNode(env, instance, type);
    env->ReleaseStringUTFChars(_type, type);

    if (!node) {
        return 0;
    }
    return reinterpret_cast<intptr_t>(node);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_Node_addSubNode__JJ(JNIEnv *env, jobject instance, jlong mNativeNode,
                                                   jlong subNativeNode) {

    // TODO

}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_node_Node_setFrame__JIIII(JNIEnv *env, jobject instance,
                                                    jlong nativeNode,
                                                    jint x, jint y, jint width, jint height) {

    GCanvasNode *node = reinterpret_cast<GCanvasNode *>(nativeNode);
    node->setFrame(Rect(x, y, width, height));

}

}