//
// Created by neo on 2018/5/17.
//

#include "JSObject.h"
#include "AtomJSLog.h"
#include <string>
#include <mutex>
#include <JavaScriptCore/JavaScript.h>
#include <cstdlib>
#include <math.h>
#include <cstring>

using namespace std;

namespace AtomGraphicsJSC {

    map<JSObjectRef, JSObject *> JSObject::jObjMap = map<JSObjectRef, JSObject *>();
    map<long, JSObject *> JSObject::objRetainMap = map<long, JSObject *>();

    size_t JSObject::ConvertJSStringToString(JSStringRef jsString, char **result) {
        size_t size = JSStringGetMaximumUTF8CStringSize(jsString);
        *result = new char[size + 1];
        JSStringGetUTF8CString(jsString, *result, size + 1);
        return size;
    }

    JSValueRef JSObject::JSCFunctionCallback(JSContextRef ctx, JSObjectRef functionRef,
                                             JSObjectRef thisObjectRef,
                                             size_t argumentCount, JSValueRef const *arguments,
                                             JSValueRef *exception) {

        JSObject *functionObject = getReferenceObject(functionRef);
        if (functionObject == nullptr || !functionObject->isFunction()) {
            return JSValueMakeUndefined(ctx);
        }
        JSContext *context = functionObject->m_context;

        JSObject *thisObject = ConvertJSValueRefToJSObject(context, thisObjectRef, exception);
        JSObject *objects[argumentCount];
        for (size_t i = 0; i < argumentCount; i++) {
            JSValueRef value = arguments[i];
            objects[i] = ConvertJSValueRefToJSObject(context, value, exception);
        }

        JSObject *returnValue = functionObject->m_function(context, thisObject, argumentCount,
                                                           (JSObject **) objects);

        for (auto object:objects) {
            if (!object->isObject()) {
                delete object;
            }
        }

        if (returnValue != nullptr) {
            JSValueRef returnValueRef = returnValue->m_jsValueRef ? returnValue->m_jsValueRef
                                                                  : returnValue->m_jsObjectRef;
            if (!returnValue->isObject()) {
                delete returnValue;
            }
            return returnValueRef;
        } else {
            return JSValueMakeUndefined(ctx);
        }
    }

    bool JSObject::OnSetProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName,
                                 JSValueRef value, JSValueRef *exception) {
        char *propertyNameStr;
        ConvertJSStringToString(propertyName, &propertyNameStr);

        JSObject *thisObject = JSObject::getReferenceObject(object);
        JSContext *context = thisObject->m_context;
        JSObject *valueObj = ConvertJSValueRefToJSObject(thisObject->m_context, value, exception);

        const JSObject *referenceValue = thisObject->m_propertyMap[propertyNameStr];
        if (referenceValue) {
            delete thisObject->m_propertyMap[propertyNameStr];
            thisObject->m_propertyMap[std::string(propertyNameStr)] = valueObj;
        } else {
            thisObject->m_propertySetter(context, propertyNameStr, valueObj);
        }

        if (!valueObj->isObject()) {
            delete valueObj;
        }

        delete propertyNameStr;
        return true;
    }

    JSValueRef JSObject::OnGetProperty(JSContextRef ctx, JSObjectRef object,
                                       JSStringRef propertyName, JSValueRef *exception) {
        JSObject *thisObject = JSObject::getReferenceObject(object);

        char *propertyNameStr;
        ConvertJSStringToString(propertyName, &propertyNameStr);

        const JSObject *returnValue = thisObject->m_propertyMap[propertyNameStr];
        if (returnValue) {
            return returnValue->m_jsValueRef ? returnValue->m_jsValueRef
                                             : returnValue->m_jsObjectRef;
        }

        returnValue = thisObject->m_propertyGetter(thisObject->m_context, propertyNameStr);
        delete propertyNameStr;
        return returnValue == nullptr ? NULL : (returnValue->m_jsValueRef
                                                ? returnValue->m_jsValueRef
                                                : returnValue->m_jsObjectRef);
    }

    JSObject *
    JSObject::ConvertJSValueRefToJSObject(JSContext *ctx, JSValueRef value, JSValueRef *exception) {
        JSObject *object = nullptr;
        JSContextRef contextRef = ctx->getContextRef();
        switch (JSValueGetType(contextRef, value)) {
            case kJSTypeObject: {
                JSObjectRef jsObject = JSValueToObject(contextRef, value, exception);
                object = getReferenceObject(jsObject);
                if (!object) {
                    object = new JSObject(ctx, jsObject);
                    if (JSObjectIsFunction(ctx->getContextRef(), jsObject)) {
                        object->_isFunction = true;
                    }
                    object->_isObject = true;
                }
            }
                break;
            case kJSTypeString: {
                JSStringRef jsString = JSValueToStringCopy(contextRef, value, exception);
                char *strBuffer;
                ConvertJSStringToString(jsString, &strBuffer);
                object = new JSObject(ctx, strBuffer);
                delete strBuffer;
                object->m_jsValueRef = value;
                object->_isString = true;
                object->_isObject = false;
            }
                break;
            case kJSTypeNumber: {
                object = new JSObject(ctx, value);
                object->m_numberValue = JSValueToNumber(contextRef, value, exception);
                if (isnan(object->m_numberValue)) {
                    object->m_numberValue = 0;
                }
                object->_isNumber = true;
                object->_isObject = false;
            }
                break;
            case kJSTypeBoolean: {
                object = new JSObject(ctx, value);
                object->m_numberValue = JSValueToNumber(contextRef, value, exception);
                object->_isBoolean = true;
                object->_isObject = false;
            }
                break;
            case kJSTypeNull: {
                object = new JSObject(ctx, value);
                object->_isNull = true;
                object->_isObject = false;
            }
                break;
            case kJSTypeUndefined: {
                object = new JSObject(ctx, value);
                object->_isUndefined = true;
                object->_isObject = false;
            }
                break;
            default: {

            }
        }

        return object;
    }


    JSObject *JSObject::getReferenceObject(const JSObjectRef jsObject) {
        JSObject *object = nullptr;
        static std::mutex signalMutex;
        signalMutex.lock();
        object = jObjMap[jsObject];
        signalMutex.unlock();
        return object;
    }

    JSObject *JSObject::getCachedObject(void *object) {
        long ptr = reinterpret_cast<intptr_t>(object);
        static std::mutex signalMutex;
        signalMutex.lock();
        JSObject *jsObject = objRetainMap[ptr];
        signalMutex.unlock();
        return jsObject;
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, void *nativeObject, JSClassDefinition definition) {
        return CreateObject(ctx, nativeObject, nullptr, nullptr, definition);
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, void *nativeObject, JSSetPropertyCallback propertySetter,
                                     JSGetPropertyCallback propertyGetter, JSClassDefinition definition) {
        if (nativeObject != nullptr) {
            JSObject *cachedObj = objRetainMap[reinterpret_cast<intptr_t>(nativeObject)];
            if (cachedObj) {
                return cachedObj;
            }
        }

        return new JSObject(ctx, nativeObject, propertySetter, propertyGetter, definition);
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, const char *stringBuffer) {
        return new JSObject(ctx, stringBuffer);
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, double numberValue) {
        return new JSObject(ctx, numberValue);
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, JSObjectRef jsObjectRef) {
        return new JSObject(ctx, jsObjectRef);
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, JSValueRef jsValueRef) {
        return new JSObject(ctx, jsValueRef);
    }

    JSObject *
    JSObject::CreateObject(JSContext *ctx, std::string functionName, JSFunctionObjectCallback functionValue) {
        return new JSObject(ctx, functionName, functionValue);
    }

    JSObject *JSObject::CreateObject(JSContext *ctx, std::string functionName,
                                     JSFunctionObjectCallbackWithoutReturn functionValue) {
        return new JSObject(ctx, functionName, functionValue);
    }

    JSObject::JSObject(JSContext *ctx, void *nativeObject, JSClassDefinition definition)
            : JSObject(ctx, nativeObject, nullptr, nullptr, definition) {

    }

    JSObject::JSObject(JSContext *ctx, double numberValue) {
        _isNumber = true;
        m_context = ctx;
        m_numberValue = numberValue;
        m_jsValueRef = JSValueMakeNumber(ctx->getContextRef(), numberValue);
    }


    JSObject::JSObject(JSContext *ctx, const char *stringBuffer) {
        m_context = ctx;
        m_stringBuffer = new char[strlen(stringBuffer) + 1];
        strcpy(m_stringBuffer, stringBuffer);
        _isString = true;
        m_jsValueRef = JSValueMakeString(ctx->getContextRef(),
                                         JSStringCreateWithUTF8CString(stringBuffer));
    }

    JSObject::JSObject(JSContext *ctx, JSObjectRef jsObjectRef) {
        m_context = ctx;
        m_jsObjectRef = jsObjectRef;
        JSValueProtect(ctx->getContextRef(), m_jsObjectRef);
    }

    JSObject::JSObject(JSContext *ctx, JSValueRef jsValueRef) {
        m_context = ctx;
        m_jsValueRef = jsValueRef;
        JSValueProtect(ctx->getContextRef(), m_jsValueRef);
    }

    JSObject::JSObject(JSContext *ctx, void *nativeObject, JSSetPropertyCallback propertySetter,
                       JSGetPropertyCallback propertyGetter, JSClassDefinition definition) {
        m_context = ctx;
        m_dataPointer = nativeObject;

        if (propertyGetter) {
            this->m_propertyGetter = propertyGetter;
            definition.getProperty = OnGetProperty;
        }

        if (propertySetter) {
            this->m_propertySetter = propertySetter;
            definition.setProperty = OnSetProperty;
        }

        JSClassRef classRef = JSClassCreate(&definition);
        m_jsObjectRef = JSObjectMake(ctx->getContextRef(), classRef, nullptr);
        _isObject = nativeObject != nullptr;
        if (_isObject) {
            m_propertyMap = std::map<std::string, const JSObject *>();
            static std::mutex signalMutex;
            signalMutex.lock();
            jObjMap[m_jsObjectRef] = this;
            objRetainMap[reinterpret_cast<intptr_t>(nativeObject)] = this;
            signalMutex.unlock();
            JSValueProtect(m_context->getContextRef(), m_jsObjectRef);
        }
    }

    JSObject::JSObject(JSContext *ctx, std::string name, JSFunctionObjectCallback functionValue) {
        JSStringRef functionName = JSStringCreateWithUTF8CString(name.data());

        m_jsObjectRef = JSObjectMakeFunctionWithCallback(ctx->getContextRef(), functionName,
                                                         JSCFunctionCallback);
        m_function = functionValue;
        m_context = ctx;
        _isFunction = true;

        static std::mutex signalMutex;
        signalMutex.lock();
        jObjMap[m_jsObjectRef] = this;
        signalMutex.unlock();
        JSValueProtect(m_context->getContextRef(), m_jsObjectRef);
    }

    JSObject::JSObject(JSContext *ctx, std::string name,
                       JSFunctionObjectCallbackWithoutReturn functionValue)
            : JSObject(ctx, name, (JSFunctionObjectCallback) ([functionValue](
            JSContext *ctx, JSObject *thisObject, size_t argumentCount, JSObject *arguments[]) {

        functionValue(ctx, thisObject, argumentCount, arguments);
        return new JSObject(ctx, JSValueMakeUndefined(ctx->getContextRef()));
    })) {

    }


    JSObject::~JSObject() {
        if (_isObject) {
            static std::mutex signalMutex;
            signalMutex.lock();
            jObjMap.erase(m_jsObjectRef);
            signalMutex.unlock();
            m_propertyMap.clear();
            JSValueUnprotect(m_context->getContextRef(), m_jsObjectRef);
        } else if (_isString && m_stringBuffer != nullptr) {
            delete m_stringBuffer;
        }
        if (m_jsValueRef != NULL) {
            JSValueUnprotect(m_context->getContextRef(), m_jsValueRef);
        }
    }

    void JSObject::setProperty(std::string propertyName, const JSObject *value) {
        if (m_propertyGetter) {
            m_propertyMap[propertyName] = value;
        } else {
            JSValueRef exception = nullptr;
            JSStringRef propertyNameJS = JSStringCreateWithUTF8CString(propertyName.data());
            JSValueRef propertyValue = value->m_jsValueRef ? value->m_jsValueRef
                                                           : value->m_jsObjectRef;
            JSObjectSetProperty(m_context->getContextRef(), m_jsObjectRef, propertyNameJS,
                                propertyValue,
                                kJSClassAttributeNone, &exception);
            if (exception != nullptr) {
                AtomJSLog::ShowException(m_context->getContextRef(), exception);
            }
        }
    }

    void JSObject::setProperty(std::string propertyName, JSFunctionObjectCallback callback) {

        JSObject *functionObject = new JSObject(m_context, propertyName, callback);

        if (m_propertySetter) {
            m_propertyMap[propertyName] = functionObject;
        } else {
            JSStringRef functionName = JSStringCreateWithUTF8CString(propertyName.data());

            JSValueRef exception = nullptr;
            JSObjectSetProperty(m_context->getContextRef(), m_jsObjectRef, functionName,
                                functionObject->m_jsObjectRef,
                                kJSClassAttributeNone, &exception);
            if (exception != nullptr) {
                AtomJSLog::ShowException(m_context->getContextRef(), exception);
            }
        }
    }


    void JSObject::setProperty(std::string propertyName,
                               JSFunctionObjectCallbackWithoutReturn callbackWithoutReturn) {

        JSObject *functionObject = new JSObject(m_context, propertyName, callbackWithoutReturn);

        if (m_propertySetter) {
            m_propertyMap[propertyName] = functionObject;
        } else {
            JSStringRef functionName = JSStringCreateWithUTF8CString(propertyName.data());

            JSValueRef exception = nullptr;
            JSObjectSetProperty(m_context->getContextRef(), m_jsObjectRef, functionName,
                                functionObject->m_jsObjectRef,
                                kJSClassAttributeNone, &exception);
            if (exception != nullptr) {
                AtomJSLog::ShowException(m_context->getContextRef(), exception);
            }

        }

    }

    const JSObject *JSObject::getProperty(std::string propertyName) {
        if (m_propertyGetter) {
            return m_propertyMap[propertyName];
        } else {
            JSStringRef jsPropertyName = JSStringCreateWithUTF8CString(propertyName.c_str());
            JSValueRef exception;
            JSValueRef value = JSObjectGetProperty(m_context->getContextRef(), m_jsObjectRef,
                                                   jsPropertyName, &exception);
            if (exception) {
                AtomJSLog::ShowException(m_context->getContextRef(), exception);
                return nullptr;
            }

            JSObject *object = ConvertJSValueRefToJSObject(m_context, value, &exception);
            if (exception) {
                AtomJSLog::ShowException(m_context->getContextRef(), exception);
                return nullptr;
            }

            return object;
        }
    }

    const JSObject *JSObject::callAsFunction(JSObject *thisObject) {
        std::vector<JSObject *> arguments;
        return callAsFunctionWithArguments(thisObject, arguments);
    }


    const JSObject *
    JSObject::callAsFunctionWithArguments(JSObject *thisObject, std::vector<JSObject *> arguments) {
        if (_isFunction) {
            size_t argumentCount = arguments.size();
            JSValueRef valueArguments[argumentCount];
            for (int i = 0; i < argumentCount; i++) {
                JSObject *argument = arguments[i];
                valueArguments[i] = argument->m_jsValueRef ? argument->m_jsValueRef
                                                           : argument->m_jsObjectRef;
            }
            JSValueRef exception = nullptr;
            JSValueRef returnValue =
                    JSObjectCallAsFunction(m_context->getContextRef(), m_jsObjectRef,
                                           thisObject->m_jsObjectRef, argumentCount, valueArguments,
                                           &exception);
            if (exception) {
                AtomJSLog::ShowException(m_context->getContextRef(), exception);
            } else {
                JSObject *returnValueObject = ConvertJSValueRefToJSObject(m_context, returnValue,
                                                                          &exception);
                if (exception) {
                    AtomJSLog::ShowException(m_context->getContextRef(), exception);
                } else {
                    return returnValueObject;
                }
            }
        }
        return nullptr;
    }

    bool JSObject::isObject() const {
        return _isObject;
    }

    bool JSObject::isString() const {
        return _isString;
    }

    bool JSObject::isNumber() const {
        return _isNumber;
    }

    bool JSObject::isBoolean() const {
        return _isBoolean;
    }

    bool JSObject::isNull() const {
        return _isNull;
    }

    bool JSObject::isUndefined() const {
        return _isUndefined;
    }

    bool JSObject::isFunction() const {
        return _isFunction;
    }
}