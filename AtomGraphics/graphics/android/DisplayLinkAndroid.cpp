//
// Created by neo on 2018/7/26.
//

#include "android/AtomGraphicsJNIHelper.h"
#include "DisplayLinkAndroid.h"

namespace AtomGraphics {

    DisplayLinkAndroid::DisplayLinkAndroid(GraphicsContentFlushController *flushController,
                                           GraphicsPageContext *pageContext)
            : DisplayLink(flushController) {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz_displaylink = AtomGraphicsBase::FindClass(env, pageContext->getJContext(),
                                                               "com/neo/atomgraphics/DisplayLink");
        jmethodID method_id = env->GetMethodID(clazz_displaylink, "<init>", "(JJ)V");
        jlong displayLinkPtr = reinterpret_cast<intptr_t>(this);
        jlong pageContextPtr = reinterpret_cast<intptr_t>(pageContext);
        jobject displayLinkRef = env->NewObject(clazz_displaylink, method_id, displayLinkPtr, pageContextPtr);
        m_displayLinkObj = env->NewGlobalRef(displayLinkRef);
    }

    void DisplayLinkAndroid::schedule() {
        if (m_pause) {
            m_pause = false;
            JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
            jclass clazz = env->GetObjectClass(m_displayLinkObj);
            jmethodID method_id = env->GetMethodID(clazz, "schedule", "()V");
            env->CallVoidMethod(m_displayLinkObj, method_id);
        }
    }

    void DisplayLinkAndroid::pause() {
        m_pause = true;
    }
}
