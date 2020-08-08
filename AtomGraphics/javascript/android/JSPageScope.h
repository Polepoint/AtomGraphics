//
// Created by neo on 2019/3/14.
//

#ifndef ATOMGRAPHICS_JSPAGESCOPE_H
#define ATOMGRAPHICS_JSPAGESCOPE_H

#include <string>
#include <vector>
#include <v8/v8.h>
#include <jni.h>
#include "javascript/v8/V8JavaScriptInterface.h"
#include "JSMessageHandler.h"

namespace AtomGraphics {

namespace JS {

class JSPageScope {

public:

    JSPageScope(long pageID, std::string srcFileContent, V8JavaScriptInterface *v8JSInterface);

    void destroy(V8JavaScriptInterface *v8JSInterface);

    void evaluateScript(std::string script, V8JavaScriptInterface *v8JSInterface);

    void callJSHandler(std::string handlerName, const char *params, V8JavaScriptInterface *v8JSInterface);

    void registerNativeHandler(std::string handlerName, jobject handler, V8JavaScriptInterface *v8JSInterface);

    long getPageID() const {
        return m_pageID;
    }

private:
    long m_pageID{0};

    std::vector<std::unique_ptr<JSMessageHandler>> m_handlers;

    v8::Global<v8::Object> m_pageScope;
    v8::Global<v8::Function> m_pageScopeEvaluatorFunc;
    v8::Global<v8::Function> m_pageScopeRegisterNativeHandler;
    v8::Global<v8::Function> m_pageScopeCallJSHandler;
};

}

}


#endif //ATOMGRAPHICS_JSPAGESCOPE_H
