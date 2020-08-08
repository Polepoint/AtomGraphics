//
// Created by neo on 2018/10/16.
//

#include <v8/libplatform/libplatform.h>
#include "javascript/ScriptTimerController.h"
#include "javascript/android/JSPageScopeTracer.h"
#include "javascript/ScriptAnimationFrameController.h"
#include "graphics/GraphicsContentFlushController.h"
#include "base/Device.h"
#include "base/AtomLog.h"
#include "V8JSContext.h"
#include "V8Platform.h"
#include "JSExecutionChecker.h"
#include "V8JSDocument.h"

using namespace v8;

namespace AtomGraphics {

namespace JS {

V8JSContext::V8JSContext() {
    Isolate *isolate = V8Platform::SharedPlatform()->getIsolate();
    HandleScope handleScope(isolate);
    static Global<ObjectTemplate> globalTemplate;
    if (globalTemplate.IsEmpty()) {
        Local<ObjectTemplate> templ = ObjectTemplate::New(isolate);
        templ->SetInternalFieldCount(1);
        globalTemplate.Reset(isolate, templ);
    }

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, globalTemplate);
    v8::Local<v8::Context> context = Context::New(isolate, nullptr, templ);
    Local<External> context_ptr = External::New(isolate, this);
    context->Global()->SetInternalField(0, context_ptr);
    m_context.Reset(isolate, context);
}

V8JSContext::~V8JSContext() {}

Local<Value> V8JSContext::evaluateScript(std::string script, Isolate *isolate) {
    TryCatch try_catch(isolate);
    Local<Context> context = m_context.Get(isolate);
    EscapableHandleScope handle_scope(isolate);

    Local<v8::String> source;
    if (!v8::String::NewFromUtf8(isolate, script.c_str(), NewStringType::kNormal, script.length()).ToLocal(&source)) {
        v8::String::Utf8Value error(isolate, try_catch.Exception());
        AtomLogError("AtomGraphic JS Error: %s", *error);
        return handle_scope.Escape(try_catch.Exception());
    }

    // Compile the script and check for errors.
    Local<Script> compiled_script;
    if (!Script::Compile(context, source).ToLocal(&compiled_script)) {
        v8::String::Utf8Value error(isolate, try_catch.Exception());
        AtomLogError("AtomGraphic JS Error: %s", *error);
        return handle_scope.Escape(try_catch.Exception());
    }

    // Run the script!
    Local<Value> result;
    if (!compiled_script->Run(context).ToLocal(&result)) {
        v8::String::Utf8Value error(isolate, try_catch.Exception());
        AtomLogError("AtomGraphic JS Error: %s", *error);
        Local<Value> stack_trace;
        if (try_catch.StackTrace(context).ToLocal(&stack_trace)) {
            v8::String::Utf8Value stackTrace(isolate, stack_trace);
            AtomLogError("Stack: %s", *stackTrace);
        }

        return handle_scope.Escape(try_catch.Exception());
    }

    return handle_scope.Escape(result);
}


void SetTimeoutCallback(const FunctionCallbackInfo<Value> &info) {
    if (info.Length() < 1) {
        return;
    }

    Local<Value> function_val = info[0];
    if (!function_val->IsFunction()) {
        return;
    }

    JSPageScope *pageScope = JSPageScopeTracer::CurrentPageScope();
    JSPageScopeTracer pageScopeTracer(pageScope);

    Isolate *isolate = info.GetIsolate();
    Local<Function> function = Local<Function>::Cast(function_val);
    auto *func = new Global<Function>(isolate, function);
    long delay = static_cast<long>(info[1]->ToInteger()->Value());
    double timerID = ScriptTimerController::CreateTimerID(delay, false, [func, isolate, pageScope] {
        JSPageScopeTracer tracer(pageScope);
        HandleScope handle_scope(isolate);
        func->Get(isolate)->Call(
                isolate->GetCurrentContext(),
                isolate->GetCurrentContext()->Global(),
                0,
                nullptr);
    }, func);

    info.GetReturnValue().Set(timerID);
}

void ClearTimeoutCallback(const FunctionCallbackInfo<Value> &info) {
    if (info.Length() <= 0) {
        return;
    }
    long timerID = static_cast<long>(info[0]->ToNumber(info.GetIsolate())->Value());
    ScriptTimerController::RemoveTimerID(timerID);
}

void SetIntervalCallback(const FunctionCallbackInfo<Value> &info) {
    if (info.Length() < 1) {
        return;
    }

    Local<Value> function_val = info[0];
    if (!function_val->IsFunction()) {
        return;
    }

    JSPageScope *pageScope = JSPageScopeTracer::CurrentPageScope();
    JSPageScopeTracer pageScopeTracer(pageScope);

    Isolate *isolate = info.GetIsolate();
    Local<Function> function = Local<Function>::Cast(function_val);
    auto *func = new Global<Function>(isolate, function);
    long delay = static_cast<long>(info[1]->ToInteger()->Value());
    double timerID = ScriptTimerController::CreateTimerID(delay, true, [func, isolate, pageScope]() {
        JSPageScopeTracer tracer(pageScope);
        HandleScope handle_scope(isolate);
        Local<Function> function_local = func->Get(isolate);
        if (function_local->IsFunction()) {
            function_local->Call(
                    isolate->GetCurrentContext(),
                    isolate->GetCurrentContext()->Global(),
                    0,
                    nullptr);
        }
    }, func);

    info.GetReturnValue().Set(timerID);
}

void ClearIntervalCallback(const FunctionCallbackInfo<Value> &info) {
    if (info.Length() < 1) {
        return;
    }
    long timerID = static_cast<long>(info[0]->ToNumber(info.GetIsolate())->Value());
    ScriptTimerController::RemoveTimerID(timerID);
}

void V8JSContext::RequestAnimationFrameCallback(const FunctionCallbackInfo<Value> &info) {
    if (info.Length() < 1) {
        return;
    }
    Local<Value> function_val = info[0];
    if (!function_val->IsFunction()) {
        return;
    }

    JSPageScope *pageScope = JSPageScopeTracer::CurrentPageScope();
    JSPageScopeTracer pageScopeTracer(pageScope);

    Isolate *isolate = info.GetIsolate();
    Local<Function> function = Local<Function>::Cast(function_val);
    auto *func = new Global<Function>(isolate, function);
    double callbackID = ScriptAnimationFrameController::SharedInstance()->addCallback(
            RequestAnimationCallback::Create(
            [func, isolate, pageScope]() {
                JSPageScopeTracer tracer(pageScope);
                HandleScope handle_scope(isolate);
                Local<Function> function_local = func->Get(isolate);
                if (function_local->IsFunction()) {
                    v8::Local<v8::Value> result;
                    Local<Context> context = isolate->GetCurrentContext();
                    CheckFunctionCall(context, function_local->Call(context, context->Global(), 0, nullptr), result);
                }
                delete func;
            }));
    GraphicsContentFlushController::SharedInstance()->scheduleLayerFlush();
    info.GetReturnValue().Set(callbackID);

}

void V8JSContext::CancelAnimationFrameCallback(const FunctionCallbackInfo<Value> &info) {
    if (info.Length() < 1) {
        return;
    }
    long callbackID = static_cast<long>(info[0]->ToNumber(info.GetIsolate())->Value());
    ScriptAnimationFrameController::SharedInstance()->cancelCallback(callbackID);
}

void V8JSContext::setupWindowMethods(Isolate *isolate) {
    Local<Context> context(isolate->GetCurrentContext());
    if (context.IsEmpty()) {
        return;
    }

    Local<Object> globalObj = context->Global();
    Local<Value> window_val = globalObj->Get(v8::String::NewFromUtf8(isolate, "window",
                                                                     NewStringType::kNormal).ToLocalChecked());
    if (window_val.IsEmpty()) {
        globalObj->Set(v8::String::NewFromUtf8(isolate, "window"), globalObj);
        window_val = globalObj;
    }

    Local<Object> windowObj = Local<Object>::Cast(window_val);
    windowObj->Set(v8::String::NewFromUtf8(isolate, "setTimeout", NewStringType::kNormal).ToLocalChecked(),
                   v8::Function::New(isolate, SetTimeoutCallback));
    windowObj->Set(v8::String::NewFromUtf8(isolate, "clearTimeout", NewStringType::kNormal).ToLocalChecked(),
                   v8::Function::New(isolate, ClearTimeoutCallback));
    windowObj->Set(v8::String::NewFromUtf8(isolate, "setInterval", NewStringType::kNormal).ToLocalChecked(),
                   v8::Function::New(isolate, SetIntervalCallback));
    windowObj->Set(v8::String::NewFromUtf8(isolate, "clearInterval", NewStringType::kNormal).ToLocalChecked(),
                   v8::Function::New(isolate, ClearIntervalCallback));
    windowObj->Set(v8::String::NewFromUtf8(isolate, "requestAnimationFrame", NewStringType::kNormal).ToLocalChecked(),
                   v8::Function::New(isolate, RequestAnimationFrameCallback));
    windowObj->Set(v8::String::NewFromUtf8(isolate, "cancelAnimationFrame", NewStringType::kNormal).ToLocalChecked(),
                   v8::Function::New(isolate, CancelAnimationFrameCallback));
    windowObj->Set(v8::String::NewFromUtf8(isolate, "devicePixelRatio", NewStringType::kNormal).ToLocalChecked(),
                   v8::Number::New(isolate, Device::DeviceScaleFactor()));

    V8JSDocument::SetupDocument(isolate, globalObj);
}

} // namespace JS

} // namespace AtomGraphics