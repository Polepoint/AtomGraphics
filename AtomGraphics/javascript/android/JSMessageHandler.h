//
// Created by neo on 2019/3/15.
//

#ifndef ATOMGRAPHICS_JSMESSAGEHANDLER_H
#define ATOMGRAPHICS_JSMESSAGEHANDLER_H

#include <jni.h>
#include <v8/v8.h>
#include "javascript/v8/V8JavaScriptInterface.h"

namespace AtomGraphics {

namespace JS {

class JSMessageHandler final {

public:

    static v8::Local<v8::Object> WrapMessageHandler(
            JSMessageHandler *handler,
            v8::Isolate *isolate,
            v8::Local<v8::Context> context);

    JSMessageHandler(V8JavaScriptInterface *v8JSInterface, jobject jRef);

    ~JSMessageHandler();

    void callMessage(const char *data, v8::Local<v8::Object> *recv, v8::Local<v8::Function> *callback);

private:

    V8JavaScriptInterface *m_interface;
    jobject m_jRef{nullptr};

    static void Handle(const v8::FunctionCallbackInfo<v8::Value> &info);

    static v8::Local<v8::ObjectTemplate> MakeCallbackFunctionTemplate(v8::Isolate *isolate);

    static JSMessageHandler *UnwrapMessageHandler(v8::Local<v8::Object> obj);

};

}

}


#endif //ATOMGRAPHICS_JSMESSAGEHANDLER_H
