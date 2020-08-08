//
// Created by neo on 2019-06-23.
//

#ifndef ATOMGRAPHICS_JSEXECUTIONCHECKER_H
#define ATOMGRAPHICS_JSEXECUTIONCHECKER_H


#include "base/AtomLog.h"
#include <v8/v8.h>

#define CheckFunctionCall(context, functionCallExpression, result) \
{ \
v8::Isolate *isolate = context->GetIsolate();\
v8::TryCatch try_catch(isolate); \
if (!functionCallExpression.ToLocal(&result)) { \
    v8::String::Utf8Value error(isolate, try_catch.Exception()); \
    AtomLogError("AtomGraphic JS Error: %s", *error); \
    v8::Local<v8::Value> stack_trace; \
    if (try_catch.StackTrace(context).ToLocal(&stack_trace)) { \
        v8::String::Utf8Value stackTrace(isolate, stack_trace); \
        AtomLogError("Stack: %s", *stackTrace); \
    } \
} \
} \


#endif //ATOMGRAPHICS_JSEXECUTIONCHECKER_H
