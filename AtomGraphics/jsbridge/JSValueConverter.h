//
// Created by neo on 2018/5/15.
//

#ifndef ATOMGRAPHICS_JSVALUECONVERTOR_H
#define ATOMGRAPHICS_JSVALUECONVERTOR_H

#include <JavaScriptCore/JSValueRef.h>
#include <cstring>
#include <string>
#include <JavaScriptCore/JavaScript.h>
#include <typeinfo>
#include <functional>
#include "JSObject.h"
#include "AtomJSLog.h"


namespace AtomGraphicsJSC {

    using namespace std;

    static JSValueRef makeJSValue(JSContextRef ctx, int intValue) {
        return JSValueMakeNumber(ctx, intValue);
    }

    static JSValueRef makeJSValue(JSContextRef ctx, long longValue) {
        return JSValueMakeNumber(ctx, longValue);
    }

    static JSValueRef makeJSValue(JSContextRef ctx, float floatValue) {
        return JSValueMakeNumber(ctx, floatValue);
    }

    static JSValueRef makeJSValue(JSContextRef ctx, double doubleValue) {
        return JSValueMakeNumber(ctx, doubleValue);
    }

    static JSValueRef makeJSValue(JSContextRef ctx, bool boolValue) {
        return JSValueMakeBoolean(ctx, boolValue);
    }

    static JSValueRef makeJSValue(JSContextRef ctx, const char *charValue) {
        return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(charValue));
    }

    static JSValueRef makeJSValue(JSContextRef ctx, string stringValue) {
        return makeJSValue(ctx, stringValue.data());
    }

    template<typename T>
    static JSValueRef
    makeJSValue(JSContextRef ctx, size_t elementCount, const JSObject *value[]) {
        JSValueRef jsValue = nullptr;
        JSValueRef exception = nullptr;
        JSValueRef jsValues[elementCount];
        for (int i = 0; i < elementCount; i++) {
            const JSObject *object = value[i];
            jsValues[i] = object->toJSObjectRef();
        }
        jsValue = JSObjectMakeArray(ctx, elementCount, jsValues, &exception);
        if (exception) {
            AtomJSLog::ShowException(ctx, exception);
        }

        return jsValue;
    }

    /**
     * std::function<void()> a = []{};
     * @tparam T
     * @param ctx
     * @param function
     * @return
     */
    template<typename T>
    static JSValueRef makeJSValue(JSContextRef ctx, function<T> functionValue) {
        return nullptr;
    }

    template<typename T>
    static JSValueRef
    makeJSDateValue(JSContextRef ctx, size_t argumentCount, const JSObject *arguments[]) {
        JSValueRef exception = nullptr;
        JSValueRef jsValues[argumentCount];
        for (int i = 0; i < argumentCount; i++) {
            const JSObject *object = arguments[i];
            jsValues[i] = object->toJSObjectRef();
        }
        JSValueRef value = JSObjectMakeDate(ctx, argumentCount, jsValues, &exception);
        if (exception != nullptr) {
            AtomJSLog::ShowException(ctx, exception);
        }

        return value;
    }

    static JSObject* ConvertJSValueRefToJSObject();
}

#endif //ATOMGRAPHICS_JSVALUECONVERTOR_H
