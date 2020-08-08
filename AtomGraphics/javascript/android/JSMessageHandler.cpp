//
// Created by neo on 2019/3/15.
//

#include "android/AtomGraphicsJNIHelper.h"
#include "JSMessageHandler.h"
#include "JSMessageHandlerCallback.h"

using namespace v8;

namespace AtomGraphics {

namespace JS {

static Global<ObjectTemplate> handlerFunctionTemplate_;

Local<Object> JSMessageHandler::WrapMessageHandler(
        JSMessageHandler *handler,
        Isolate *isolate,
        Local<Context> context) {
    EscapableHandleScope handle_scope(isolate);

    if (handlerFunctionTemplate_.IsEmpty()) {
        Local<ObjectTemplate> funcTemplate = MakeCallbackFunctionTemplate(isolate);
        handlerFunctionTemplate_.Reset(isolate, funcTemplate);
    }

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, handlerFunctionTemplate_);
    Local<Object> result = templ->NewInstance(context).ToLocalChecked();
    Local<External> nativeFiled_ptr = External::New(isolate, handler);
    result->SetInternalField(0, nativeFiled_ptr);

    return handle_scope.Escape(result);
}

JSMessageHandler *JSMessageHandler::UnwrapMessageHandler(Local<Object> obj) {
    Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
    void *ptr = field->Value();
    return static_cast<JSMessageHandler *>(ptr);
}

Local<ObjectTemplate> JSMessageHandler::MakeCallbackFunctionTemplate(Isolate *isolate) {
    EscapableHandleScope handle_scope(isolate);

    Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);
    result->SetCallAsFunctionHandler(Handle);

    return handle_scope.Escape(result);
}

JSMessageHandler::JSMessageHandler(V8JavaScriptInterface *v8JSInterface, jobject jRef)
        : m_interface(v8JSInterface), m_jRef(jRef) {

}

JSMessageHandler::~JSMessageHandler() {
    if (m_jRef) {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        env->DeleteGlobalRef(m_jRef);
    }
}

void JSMessageHandler::callMessage(const char *data, Local<Object> *recv, Local<Function> *callback) {
    JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
    jclass clazz = env->GetObjectClass(m_jRef);
    jmethodID methodID = env->GetMethodID(
            clazz,
            "onMessage",
            "(Ljava/lang/String;Lcom/neo/atomgraphics/javascript/JSMessageCallback;)V");
    jstring jData = env->NewStringUTF(data);
    if (recv && callback) {
        JSMessageHandlerCallback handlerCallback(m_interface, *recv, *callback);
        jclass callbackClass = AtomGraphicsBase::FindClass(env, "com/neo/atomgraphics/javascript/JSMessageCallback");
        jmethodID method_id = env->GetMethodID(callbackClass, "<init>", "(J)V");
        jlong callbackPtr = reinterpret_cast<intptr_t>(&handlerCallback);
        jobject callbackObj = env->NewObject(callbackClass, method_id, callbackPtr);
        env->CallVoidMethod(m_jRef, methodID, jData, callbackObj);
        env->DeleteLocalRef(callbackObj);
        env->DeleteLocalRef(callbackClass);
    } else {
        env->CallVoidMethod(m_jRef, methodID, jData, nullptr);
    }
    env->DeleteLocalRef(jData);
    env->DeleteLocalRef(clazz);
}

void JSMessageHandler::Handle(const FunctionCallbackInfo<Value> &info) {
    JSMessageHandler *handler = UnwrapMessageHandler(info.Holder());
    String::Utf8Value utf8_value(info.GetIsolate(), info[0]);
    int argCount = info.Length();
    if (argCount == 1) {
        handler->callMessage(*utf8_value, nullptr, nullptr);
    } else if (argCount == 2) {
        Local<Object> recv = info.This();
        Local<Function> callback = Local<Function>::Cast(info[1]);
        handler->callMessage(*utf8_value, &recv, &callback);
    }
}

}

}