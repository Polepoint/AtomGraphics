//
// Created by neo on 2018/10/16.
//
#include <v8/v8.h>
#include <v8/v8-platform.h>
#include <v8/libplatform/libplatform.h>
#include <string>
#include "base/AtomLog.h"
#include "thread/GraphicsThread.h"
#include "android/AtomGraphicsJNIHelper.h"
#include "node/canvas/android/skia/CanvasNodeSkia.h"
#include "graphics/GraphicsPageManager.h"
#include "graphics/GraphicsLayer.h"
#include "V8JavaScriptInterface.h"
#include "V8Canvas.h"
#include "V8Platform.h"
#include "V8Image.h"

using namespace v8;

namespace AtomGraphics {

namespace JS {

static V8JavaScriptInterface *sharedInstance;

V8JavaScriptInterface *V8JavaScriptInterface::SharedInterface() {
    if (!sharedInstance) {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        jclass clazz_GraphicsJavaScriptCore =
                AtomGraphicsBase::FindClass(env, "com/neo/atomgraphics/GraphicsJavaScriptCore");
        jmethodID method_id = env->GetStaticMethodID(clazz_GraphicsJavaScriptCore, "globalJavaScriptCore",
                                                     "()Lcom/neo/atomgraphics/GraphicsJavaScriptCore;");
        env->CallStaticVoidMethod(clazz_GraphicsJavaScriptCore, method_id);
        env->DeleteLocalRef(clazz_GraphicsJavaScriptCore);
    }

    return sharedInstance;
}

V8JavaScriptInterface::V8JavaScriptInterface(jobject interfaceRef)
        : m_interfaceRef(interfaceRef) {
    sharedInstance = this;
}


Local<Value> V8JavaScriptInterface::evaluateScript(const char *script) {
    if (!this->m_apiInitialized) {
        this->m_apiInitialized = true;
        this->initAPIs();
    }
    Isolate *isolate = getIsolate();
    EscapableHandleScope handle_scope(isolate);
    Local<Value> result = this->m_context->evaluateScript(script, isolate);
    return handle_scope.Escape(result);
}

V8JSContext *V8JavaScriptInterface::context() {
    if (!m_context) {
        m_context.reset(new V8JSContext);
    }

    return m_context.get();
}

v8::Isolate *V8JavaScriptInterface::getIsolate() {
    return V8Platform::SharedPlatform()->getIsolate();
}

void V8JavaScriptInterface::reset() {
    m_context.reset(new V8JSContext);
    m_apiInitialized = false;
}

inline std::shared_ptr<Node> GetPageRootNodeById(long pageID) {
    GraphicsPage *page = GraphicsPageManager::getPageByID(pageID);
    if (page) {
        auto rootNode = page->rootLayer()->getRootNode();
        if (!rootNode) {
            rootNode.reset(new CanvasNodeSkia);
            page->rootLayer()->setRootNode(rootNode);
        }

        return rootNode;
    }

    return nullptr;
}

void V8JavaScriptInterface::initAPIs() {


    this->preloadFiles();

    Isolate *isolate = V8Platform::SharedPlatform()->getIsolate();
    m_context->setupWindowMethods(isolate);

    Local<Context> context(isolate->GetCurrentContext());
//
//        Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
//
//        Local<Context> context = Context::New(isolate, NULL, global);

    Local<v8::String> ag_name = v8::String::NewFromUtf8(isolate, "AG", NewStringType::kNormal).ToLocalChecked();
    Local<Value> ag_val;
    // If there is no Process function, or if it is not a function,
    // bail out
    if (!context->Global()->Get(context, ag_name).ToLocal(&ag_val) ||
            !ag_val->IsObject()) {
        return;
    }

    Local<Object> ag_obj = Local<Object>::Cast(ag_val);
    ag_obj->Set(v8::String::NewFromUtf8(isolate, "getPageRootNodeById", NewStringType::kNormal).ToLocalChecked(),
                v8::Function::New(isolate, [](const FunctionCallbackInfo<Value> &args) -> void {
                    auto createNode = GetPageRootNodeById(static_cast<long>(args[0]->ToInteger()->Value()));
                    V8Canvas *canvas = V8Canvas::CreateCanvasWrappableFromCache(createNode);
                    args.GetReturnValue().Set(canvas->Wrap(args.GetIsolate(), args.Holder()->CreationContext()));
                })
    );

    ag_obj->Set(v8::String::NewFromUtf8(isolate, "createCanvasNode", NewStringType::kNormal).ToLocalChecked(),
                v8::Function::New(isolate, [](const FunctionCallbackInfo<Value> &args) -> void {
                    auto createNode = std::shared_ptr<Node>(new CanvasNodeSkia());
                    V8Canvas *canvas = V8Canvas::CreateCanvasWrappableFromCache(createNode);
                    args.GetReturnValue().Set(canvas->Wrap(args.GetIsolate(), args.Holder()->CreationContext()));
                })
    );

    ag_obj->Set(v8::String::NewFromUtf8(isolate, "log", NewStringType::kNormal).ToLocalChecked(),
                v8::Function::New(isolate, [](const FunctionCallbackInfo<Value> &args) -> void {
                    if (args.Length()) {
                        v8::String::Utf8Value utf8_value(args.GetIsolate(), args[0]);
                        AtomLogInfo("JSC log: %s", *utf8_value);
                    }
                })
    );

    ag_obj->Set(v8::String::NewFromUtf8(isolate, "createImage", NewStringType::kNormal).ToLocalChecked(),
                v8::Function::New(isolate, [](const FunctionCallbackInfo<Value> &args) -> void {
                    V8Image *image = new V8Image();
                    args.GetReturnValue().Set(image->Wrap(args.GetIsolate(), args.Holder()->CreationContext()));
                })
    );
}

void V8JavaScriptInterface::preloadFiles() {
    JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
    jclass clazz = env->GetObjectClass(m_interfaceRef);
    jmethodID methodID = env->GetMethodID(clazz, "readScriptFile", "(Ljava/lang/String;)Ljava/lang/String;");

    Isolate *isolate = getIsolate();

    const char *file1 = "core.js";
    jstring filename1 = env->NewStringUTF(file1);
    jstring scriptContent = (jstring) env->CallObjectMethod(m_interfaceRef, methodID, filename1);
    env->DeleteLocalRef(filename1);
    runScriptFileContent(isolate, env, scriptContent);
    env->DeleteLocalRef(scriptContent);

    const char *file2 = "timers.js";
    jstring filename2 = env->NewStringUTF(file2);
    scriptContent = (jstring) env->CallObjectMethod(m_interfaceRef, methodID, filename2);
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(filename2);
    runScriptFileContent(isolate, env, scriptContent);
    env->DeleteLocalRef(scriptContent);
}

void V8JavaScriptInterface::runScriptFileContent(Isolate *isolate, JNIEnv *env, jstring content) {
    const char *script = env->GetStringUTFChars(content, 0);
    EscapableHandleScope handle_scope(isolate);
    this->m_context->evaluateScript(script, isolate);
    env->ReleaseStringUTFChars(content, script);
}

} // namespace JS

} // namespace GraphicsEnvironment