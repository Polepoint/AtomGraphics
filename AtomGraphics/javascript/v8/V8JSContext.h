//
// Created by neo on 2018/10/16.
//

#ifndef ATOMGRAPHICS_V8JSCONTEXT_H
#define ATOMGRAPHICS_V8JSCONTEXT_H

#include <string>
#include <v8/v8.h>
#include <v8/v8-platform.h>
#include "javascript/ScriptAnimationFrameController.h"

namespace AtomGraphics {

namespace JS {

class V8JSContext final {

public:

    V8JSContext();

    ~V8JSContext();

    v8::Local<v8::Value> evaluateScript(std::string script, v8::Isolate *isolate);

    void setupWindowMethods(v8::Isolate *isolate);

    const v8::Global<v8::Context> &getGlobalContext() const {
        return m_context;
    };

private:

    v8::Global<v8::Context> m_context;

    static void RequestAnimationFrameCallback(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void CancelAnimationFrameCallback(const v8::FunctionCallbackInfo<v8::Value> &info);

    friend class V8JavaScriptInterface;

};

} // namespace JS

} // namespace GraphicsEnvironment


#endif //ATOMGRAPHICS_V8JSCONTEXT_H
