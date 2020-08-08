//
// Created by neo on 2019/3/17.
//

#ifndef ATOMGRAPHICS_JSMESSAGEHANDLERCALLBACK_H
#define ATOMGRAPHICS_JSMESSAGEHANDLERCALLBACK_H

#include <v8/v8.h>
#include <string>
#include "javascript/v8/V8JavaScriptInterface.h"

namespace AtomGraphics {

namespace JS {

class JSMessageHandlerCallback final {

public:

    JSMessageHandlerCallback(V8JavaScriptInterface *v8JSInterface,
                             const v8::Local<v8::Object> &recv,
                             const v8::Local<v8::Function> &callback);

    void callback(const char *data);

private:

    V8JavaScriptInterface *m_interface;
    v8::Global<v8::Object> m_recv;
    v8::Global<v8::Function> m_callback;

};

}

}


#endif //ATOMGRAPHICS_JSMESSAGEHANDLERCALLBACK_H
