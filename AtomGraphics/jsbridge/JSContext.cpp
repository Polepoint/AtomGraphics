//
// Created by neo on 2018/5/9.
//

#include <JavaScriptCore/JSStringRef.h>
#include "JSContext.h"
#include "JSObject.h"
#include "JSValueConverter.h"
#include "AtomJSLog.h"
#include <typeinfo>
#include <cstdlib>

namespace AtomGraphicsJSC {

    JSContext::JSContext() {
        JSContextGroupRef group = JSContextGroupCreate();
        m_group = JSContextGroupRetain(group);
        m_context = ((void) false, JSGlobalContextCreateInGroup(m_group, nullptr));
    }

    JSContext::~JSContext() {
        JSContextGroupRelease(m_group);
        JSGlobalContextRelease(m_context);
    }

    JSValueRef JSContext::evaluateScript(std::string script) {

        JSStringRef scriptJS = JSStringCreateWithUTF8CString(script.data());

        JSValueRef exceptionValue = nullptr;
        JSValueRef result = JSEvaluateScript(m_context, scriptJS, nullptr, nullptr, 0,
                                             &exceptionValue);
        JSStringRelease(scriptJS);
        if (exceptionValue) {
            AtomJSLog::ShowException(m_context, exceptionValue);
            return exceptionValue;
        } else {
            //TODO 像undefined这种结果，这里不应该被保护
            JSValueProtect(m_context, result);
            return result;
        }
    }

    void JSContext::setProperty(std::string propertyName, JSObject *object) {
        JSStringRef pName = JSStringCreateWithUTF8CString(propertyName.data());
        JSValueRef exception = nullptr;
        JSObjectRef globalObject = JSContextGetGlobalObject(m_context);
        JSValueRef propertyValue = object->toJSValueRef();
        propertyValue = propertyValue ? propertyValue : object->toJSObjectRef();
        JSObjectSetProperty(m_context, globalObject, pName, propertyValue,
                            kJSClassAttributeNone,
                            &exception);
        if (exception) {
            AtomJSLog::ShowException(m_context, exception);
        }
    }

    JSObject *JSContext::getProperty(std::string propertyName) {
        JSStringRef pName = JSStringCreateWithUTF8CString(propertyName.data());
        JSObjectRef globalObject = JSContextGetGlobalObject(m_context);
        JSValueRef exception = nullptr;
        JSValueRef value = JSObjectGetProperty(m_context, globalObject, pName, &exception);
        if (exception) {
            AtomJSLog::ShowException(m_context, exception);
            return nullptr;
        }

        JSObject *object = JSObject::ConvertJSValueRefToJSObject(this, value, &exception);
        if (exception) {
            AtomJSLog::ShowException(m_context, exception);
            return nullptr;
        }

        return object;
    }

    const JSContextRef JSContext::getContextRef() const {
        return m_context;
    }
}

