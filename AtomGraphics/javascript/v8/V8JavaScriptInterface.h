//
// Created by neo on 2018/10/16.
//

#ifndef ATOMGRAPHICS_V8JAVASCRIPTCOREINTERFACE_H
#define ATOMGRAPHICS_V8JAVASCRIPTCOREINTERFACE_H

#include <jni.h>
#include "V8JSContext.h"

namespace AtomGraphics {

namespace JS {

class V8JavaScriptInterface {

public:

    static V8JavaScriptInterface *SharedInterface();

    V8JavaScriptInterface(jobject interfaceRef);

    v8::Local<v8::Value> evaluateScript(const char *script);

    V8JSContext *context();

    v8::Isolate *getIsolate();

    void reset();

private:

    jobject m_interfaceRef;
    std::unique_ptr<V8JSContext> m_context;
    bool m_apiInitialized{false};

    void initAPIs();

    void preloadFiles();

    void runScriptFileContent(v8::Isolate *isolate, JNIEnv *env, jstring content);
};

} // namespace JS

} // namespace GraphicsEnvironment

#endif //ATOMGRAPHICS_V8JAVASCRIPTCOREINTERFACE_H
