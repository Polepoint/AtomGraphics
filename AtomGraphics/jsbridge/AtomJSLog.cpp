//
// Created by neo on 2018/8/16.
//

#include <cstring>
#include <android/log.h>
#include "JSObject.h"
#include "AtomJSLog.h"

#define ATOM_JSC_TAG "AtomGraphics JSC"

namespace AtomGraphicsJSC {

    void AtomGraphicsJSC::AtomJSLog::ShowException(JSContextRef context, JSValueRef exception) {
        if (JSValueIsObject(context, exception)) {
            JSValueRef valueToObjectException = nullptr;
            JSObjectRef expObject = JSValueToObject(context, exception, &valueToObjectException);
            if (valueToObjectException) {
                AtomJSLog::ShowException(context, valueToObjectException);
                return;
            }

            JSValueRef getPropertyException = nullptr;
            JSStringRef propertyName = JSStringCreateWithUTF8CString("message");
            JSValueRef expValue = JSObjectGetProperty(context, expObject, propertyName, &getPropertyException);
            if (getPropertyException) {
                AtomJSLog::ShowException(context, getPropertyException);
                return;
            }

            JSValueRef valueToStringCopy = nullptr;
            JSStringRef jsString = JSValueToStringCopy(context, expValue, &valueToStringCopy);

            if (valueToStringCopy) {
                AtomJSLog::ShowException(context, valueToStringCopy);
                return;
            }

            char *message;
            JSObject::ConvertJSStringToString(jsString, &message);
            __android_log_print(ANDROID_LOG_ERROR, ATOM_JSC_TAG, "error: %s", message);

            propertyName = JSStringCreateWithUTF8CString("stack");
            expValue = JSObjectGetProperty(context, expObject, propertyName, &getPropertyException);
            if (getPropertyException) {
                AtomJSLog::ShowException(context, getPropertyException);
                return;
            }

            jsString = JSValueToStringCopy(context, expValue, &valueToStringCopy);
            if (valueToStringCopy) {
                AtomJSLog::ShowException(context, valueToStringCopy);
                return;
            }

            char *stack;
            int stackStrSize = JSObject::ConvertJSStringToString(jsString, &stack);
            char *stackPtr = stack;
            while (stackStrSize > 0) {
                size_t lineLength = strlen(stackPtr) + 1;
                if (lineLength > 1) {
                    __android_log_print(ANDROID_LOG_ERROR, ATOM_JSC_TAG, "stack: %s", stackPtr);
                }
                stackPtr += lineLength;
                stackStrSize -= lineLength;
            }

            delete message;
            delete stack;
        }
    }
}