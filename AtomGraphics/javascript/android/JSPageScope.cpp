//
// Created by neo on 2019/3/14.
//

#include <memory>
#include "javascript/v8/JSExecutionChecker.h"
#include "thread/GraphicsThread.h"
#include "JSPageScope.h"
#include "JSMessageHandlerCallback.h"
#include "JSMessageHandler.h"
#include "JSPageScopeTracer.h"

using namespace v8;

namespace AtomGraphics {

namespace JS {

JSPageScope::JSPageScope(long pageID, std::string srcFileContent, V8JavaScriptInterface *v8JSInterface)
        : m_pageID(pageID) {

    if (srcFileContent.length()) {
        GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([this, srcFileContent, v8JSInterface]() {
            JSPageScopeTracer scopeTracer(this);
            Isolate *isolate = v8JSInterface->getIsolate();
            HandleScope handle_scope(isolate);
            Local<Value> result = v8JSInterface->evaluateScript(srcFileContent.data());
            if (!result->IsNullOrUndefined()) {
                Local<Context> context = v8JSInterface->context()->getGlobalContext().Get(isolate);

                Local<Object> initializer = result->ToObject(isolate);
                Local<Function> function = Local<Function>::Cast(initializer);
                Local<Number> pageIDValue = Number::New(isolate, this->m_pageID);
                Local<Value> args[] = {pageIDValue};
                Local<Value> scopeValue = function->Call(context, context->Global(), 1, args).ToLocalChecked();
                Local<Object> scope = Local<Object>::Cast(scopeValue);

                this->m_pageScope = Global<Object>(isolate, scope);

                Local<Value> evaluateFunc = scope->Get(
                        String::NewFromUtf8(isolate, "evaluate",
                                            NewStringType::kInternalized).ToLocalChecked());
                function = Local<Function>::Cast(evaluateFunc);
                this->m_pageScopeEvaluatorFunc = Global<Function>(isolate, function);

                Local<Value> callJSHandlerFunc = scope->Get(
                        String::NewFromUtf8(isolate, "callJSHandler",
                                            NewStringType::kInternalized).ToLocalChecked());
                function = Local<Function>::Cast(callJSHandlerFunc);
                this->m_pageScopeCallJSHandler = Global<Function>(isolate, function);

                Local<Value> registerNativeHandlerFunc = scope->Get(
                        String::NewFromUtf8(isolate, "registerNativeHandler",
                                            NewStringType::kInternalized).ToLocalChecked());

                function = Local<Function>::Cast(registerNativeHandlerFunc);
                this->m_pageScopeRegisterNativeHandler = Global<Function>(isolate, function);
            }
        });
    }
}

void JSPageScope::destroy(V8JavaScriptInterface *v8JSInterface) {
    GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([this, v8JSInterface]() {
        Isolate *isolate = v8JSInterface->getIsolate();
        HandleScope handle_scope(isolate);
        delete this;
    });
}

void JSPageScope::evaluateScript(std::string script, V8JavaScriptInterface *v8JSInterface) {
    GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([this, script, v8JSInterface]() {
        JSPageScopeTracer scopeTracer(this);
        Isolate *isolate = v8JSInterface->getIsolate();

        HandleScope scope(isolate);
        Local<Context> context = v8JSInterface->context()->getGlobalContext().Get(isolate);
        Local<Function> function_local = this->m_pageScopeEvaluatorFunc.Get(isolate);

        Local<String> data = String::NewFromUtf8(isolate, script.data(),
                                                 NewStringType::kInternalized).ToLocalChecked();
        Local<Value> argv[] = {data};
        v8::Local<v8::Value> result;
        CheckFunctionCall(context, function_local->Call(context, this->m_pageScope.Get(isolate), 1, argv), result);
    });
}

void JSPageScope::callJSHandler(std::string handlerName, const char *params, V8JavaScriptInterface *v8JSInterface) {

    bool hasParams = params != nullptr;
    std::string paramsString;
    if (hasParams) {
        paramsString = params;
    }

    GraphicsThread::
    DispatchOnGraphicsQueueWithJSContextScope([this, handlerName, hasParams, paramsString, v8JSInterface]() {
        JSPageScopeTracer scopeTracer(this);
        Isolate *isolate = v8JSInterface->getIsolate();

        HandleScope scope(isolate);
        Local<Context> context = v8JSInterface->context()->getGlobalContext().Get(isolate);
        Local<Function> function_local = this->m_pageScopeCallJSHandler.Get(isolate);

        Local<String> handlerN = String::NewFromUtf8(isolate, handlerName.data(),

                                                     NewStringType::kInternalized).ToLocalChecked();
        if (hasParams) {
            Local<String> para = String::NewFromUtf8(isolate, paramsString.data(),
                                                     NewStringType::kInternalized).ToLocalChecked();
            Local<Value> argv[] = {handlerN, para};
            v8::Local<v8::Value> result;
            CheckFunctionCall(context, function_local->Call(context, this->m_pageScope.Get(isolate), 2, argv), result);
        } else {
            Local<Value> argv[] = {handlerN};
            v8::Local<v8::Value> result;
            CheckFunctionCall(context, function_local->Call(context, this->m_pageScope.Get(isolate), 1, argv), result);
        }
    });
}

void JSPageScope::registerNativeHandler(std::string handlerName,
                                        jobject handler,
                                        V8JavaScriptInterface *v8JSInterface) {
    GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([this, handlerName, handler, v8JSInterface]() {
        JSPageScopeTracer scopeTracer(this);
        Isolate *isolate = v8JSInterface->getIsolate();

        HandleScope scope(isolate);
        Local<Context> context = v8JSInterface->context()->getGlobalContext().Get(isolate);
        Local<Function> function_local = this->m_pageScopeRegisterNativeHandler.Get(isolate);

        Local<String> handlerN = String::NewFromUtf8(isolate, handlerName.data(),
                                                     NewStringType::kInternalized).ToLocalChecked();
        auto messageHandler = new JSMessageHandler(v8JSInterface, handler);
        this->m_handlers.push_back(std::unique_ptr<JSMessageHandler>(messageHandler));
        Local<Object> handlerFunc = JSMessageHandler::WrapMessageHandler(
                messageHandler,
                isolate,
                context);

        Local<Value> argv[] = {handlerN, handlerFunc};
        v8::Local<v8::Value> result;
        CheckFunctionCall(context, function_local->Call(context, this->m_pageScope.Get(isolate), 2, argv), result);
    });
}

}

}