//
// Created by neo on 2018/8/16.
//

#ifndef ATOMGRAPHICS_ATOMJSLOG_H
#define ATOMGRAPHICS_ATOMJSLOG_H

#include <JavaScriptCore/JSValueRef.h>
#include <JavaScriptCore/JavaScript.h>

namespace AtomGraphicsJSC {

    class AtomJSLog {

    public:
        static void ShowException(JSContextRef context, JSValueRef exception);
    };
}

#endif //ATOMGRAPHICS_ATOMJSLOG_H
