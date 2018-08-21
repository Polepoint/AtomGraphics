//
// Created by neo on 2018/5/18.
//

#include <node/canvas/AtomCanvasContext2D.h>
#include <JavaScriptCore/JavaScript.h>
#include <base/AtomConvert.h>
#include "JavaScriptInterface.h"
#include <unistd.h>
#include <thread>
#include <base/AtomLog.h>
#include <graphics/GraphicsPageManager.h>
#include <node/canvas/AtomGCanvasNode.h>
#include "android/AtomGraphicsJNIHelper.h"
#include "CanvasJavaScriptAPI.h"
#include "JSTimerAPI.h"

using namespace AtomGraphics;
using namespace AtomGraphicsJSC;

inline JSObject *GetPageRootNodeById(JSContext *context, long pageID) {
    GraphicsPage *page = GraphicsPageManager::getPageByID(pageID);
    if (page) {
        CanvasNode *node = dynamic_cast<CanvasNode *>(page->rootLayer()->getRootNode());
        JSObject *jsNode = CanvasJavaScriptAPI::JSObjectForCanvasNode(context, node);
        return jsNode;
    }

    return nullptr;
}

namespace AtomGraphicsJSC {

    JavaScriptInterface::JavaScriptInterface(GraphicsPageContext *pageContext, jobject interfaceRef)
            : m_pageContext(pageContext), m_interfaceRef(interfaceRef) {

    }


    void JavaScriptInterface::initContext() {
        m_context = new JSContext();
        initAPIs();
    }

    void JavaScriptInterface::evaluateScript(const char *script) {
        char *scriptCopy = static_cast<char *>(malloc((strlen(script) + 1) * sizeof(char)));
        strcpy(scriptCopy, script);
        JavaScriptInterface *thisRef = this;
        GraphicsThread::DispatchOnGraphicsQueue([scriptCopy, thisRef]() {
            if (!thisRef->m_context) {
                thisRef->initContext();
            }
            thisRef->m_context->evaluateScript(scriptCopy);
            free(scriptCopy);
        });
    }

    void JavaScriptInterface::initAPIs() {

        this->preloadFiles();

        JSObject *agObject = m_context->getProperty("AG");
        if (!agObject || agObject->isNull() || agObject->isUndefined()) {
            agObject = new JSObject(m_context);
            m_context->setProperty("AG", agObject);
        }

        JSFunctionObjectCallback callback = [](JSContext *ctx, JSObject *thisObject,
                                               size_t argumentCount,
                                               JSObject *arguments[]) {
            JSObject *node = nullptr;
            if (argumentCount > 0) {
                node = GetPageRootNodeById(ctx, arguments[0]->toDouble());
            }
            return node;
        };

        agObject->setProperty("getPageRootNodeById", callback);

        callback = [](JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                      JSObject *arguments[]) {
            CanvasNode *createNode = new GCanvasNode();
            return CanvasJavaScriptAPI::JSObjectForCanvasNode(ctx, createNode);
        };

        agObject->setProperty("createCanvasNode", callback);


        JSFunctionObjectCallbackWithoutReturn logCallback = [](JSContext *ctx, JSObject *thisObject,
                                                               size_t argumentCount,
                                                               JSObject *arguments[]) {
            if (argumentCount) {
                const char *log = arguments[0]->toString();
                AtomLogInfo("JSC log: %s", log);
            }
        };

        agObject->setProperty("log", logCallback);

        JSObject *windowObject = m_context->getProperty("window");
        if (!windowObject || windowObject->isUndefined()) {
            windowObject = new JSObject(m_context);
            m_context->setProperty("window", agObject);
        }

        JSTimerAPI::InitJSTimerAPIs(m_context, windowObject, m_pageContext);
    }

    void JavaScriptInterface::preloadFiles() {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz = env->GetObjectClass(m_interfaceRef);
        jmethodID methodID = env->GetMethodID(clazz, "runScriptFile", "(Ljava/lang/String;)V");

        const char *file1 = "core.js";
        jstring filename1 = env->NewStringUTF(file1);
        env->CallVoidMethod(m_interfaceRef, methodID, filename1);

        const char *file2 = "JSTimers.js";
        jstring filename2 = env->NewStringUTF(file2);
        env->CallVoidMethod(m_interfaceRef, methodID, filename2);

        const char *file3 = "es6compatible.js";
        jstring filename3 = env->NewStringUTF(file3);
        env->CallVoidMethod(m_interfaceRef, methodID, filename3);

        env->DeleteLocalRef(filename1);
        env->DeleteLocalRef(filename2);
        env->DeleteLocalRef(filename3);
    }
}


