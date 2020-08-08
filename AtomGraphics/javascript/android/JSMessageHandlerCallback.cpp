//
// Created by neo on 2019/3/17.
//

#include "JSMessageHandlerCallback.h"

using namespace v8;

namespace AtomGraphics {

namespace JS {

JSMessageHandlerCallback::JSMessageHandlerCallback(V8JavaScriptInterface *v8JSInterface,
                                                   const Local<Object> &recv,
                                                   const Local<Function> &callback)
        : m_interface(v8JSInterface) {

    Isolate *isolate = v8JSInterface->getIsolate();
    m_recv.Reset(isolate, recv);
    m_callback.Reset(isolate, callback);
}

void JSMessageHandlerCallback::callback(const char *data) {
    Isolate *isolate = m_interface->getIsolate();
    if (Isolate::GetCurrent() != isolate) {
        return;
    }

    Local<Function> callback = m_callback.Get(isolate);
    Local<String> dataValue = String::NewFromUtf8(isolate, data, NewStringType::kInternalized).ToLocalChecked();
    Local<Value> args[] = {dataValue};
    callback->Call(m_interface->context()->getGlobalContext().Get(isolate), m_recv.Get(isolate), 1, args);
}

}

}