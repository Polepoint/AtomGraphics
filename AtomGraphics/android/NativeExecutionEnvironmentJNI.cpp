//
// Created by neo on 2019-09-28.
//

#include <jni.h>
#include <memory>
#include "NativeExecutionEnvironmentJNI.h"
#include "node/canvas/android/skia/CanvasNodeSkia.h"
#include "graphics/GraphicsPageManager.h"
#include "graphics/GraphicsPage.h"
#include "graphics/GraphicsLayer.h"
#include "CanvasNodeJNI.h"
#include "AtomGraphicsJNIHelper.h"


namespace AtomGraphics {

void NativeExecutionEnvironmentJNI::requestFrame(JNIEnv *env, jobject jRef) {
    if (m_frameMonitorClient->scheduling()) {
        return;
    }

    jRef = env->NewGlobalRef(jRef);

    m_frameMonitorClient->m_callbackFunction = [jRef]() {
        JNIEnv *env1 = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz_ContentView = env1->GetObjectClass(jRef);
        jmethodID methodID = env1->GetMethodID(clazz_ContentView, "doFrame", "()V");
        env1->CallVoidMethod(jRef, methodID);
        env1->DeleteGlobalRef(jRef);
        env1->DeleteLocalRef(clazz_ContentView);
    };

    m_frameMonitorClient->schedule();

    GraphicsContentFlushController::SharedInstance()->scheduleLayerFlush();
}


NativeExecutionEnvironmentJNI::FrameMonitorClient::FrameMonitorClient() {
    GraphicsContentFlushController *flushController = GraphicsContentFlushController::SharedInstance();
    flushController->refreshMonitor()->addClient(this);
}

NativeExecutionEnvironmentJNI::FrameMonitorClient::~FrameMonitorClient() {
    GraphicsContentFlushController *flushController = GraphicsContentFlushController::SharedInstance();
    flushController->refreshMonitor()->removeClient(this);
}

void NativeExecutionEnvironmentJNI::FrameMonitorClient::displayRefreshFired() {
    if (m_callbackFunction) {
        m_callbackFunction();
        m_callbackFunction = nullptr;
        m_scheduled = false;
    }
}

}


using namespace AtomGraphics;

extern "C" {


JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_execution_NativeExecutionEnvironment_nativeConstructor(JNIEnv *env,
                                                                                 jobject instance) {

    return reinterpret_cast<jlong>(new NativeExecutionEnvironmentJNI());
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_execution_NativeExecutionEnvironment_nativeDestructor(JNIEnv *env,
                                                                                jobject instance,
                                                                                jlong nativePtr) {
    delete reinterpret_cast<NativeExecutionEnvironmentJNI *>(nativePtr);
}

JNIEXPORT void JNICALL
Java_com_neo_atomgraphics_execution_NativeExecutionEnvironment_nativeRequestFrame(JNIEnv *env,
                                                                                  jobject instance,
                                                                                  jlong nativePtr) {

    auto *environmentJNI = reinterpret_cast<NativeExecutionEnvironmentJNI *>(nativePtr);
    environmentJNI->requestFrame(env, instance);
}


JNIEXPORT jlong JNICALL
Java_com_neo_atomgraphics_execution_NativeExecutionEnvironment_nativeGetRootCanvasNodeByID(JNIEnv *env,
                                                                                           jobject instance,
                                                                                           jlong pageID) {

    GraphicsPage *page = GraphicsPageManager::getPageByID(pageID);
    std::shared_ptr<Node> rootNode;
    if (page) {
        rootNode = page->rootLayer()->getRootNode();
        if (!rootNode) {
            rootNode = std::make_shared<CanvasNodeSkia>();
            page->rootLayer()->setRootNode(rootNode);
        }
    }

    return reinterpret_cast<jlong >(new CanvasNodeJNI(pageID, rootNode));

}

}