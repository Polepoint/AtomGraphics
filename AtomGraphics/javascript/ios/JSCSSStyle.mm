//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "JSCSSStyle.h"
#import "CSSToken.h"
#import "CSSParser.h"


namespace AtomGraphics {

char *JSCSSStyle::CreateStringFromJSString(JSStringRef jsString, size_t *length) {
    size_t len = JSStringGetLength(jsString) + 1;
    char *c = new char[len];
    JSStringGetUTF8CString(jsString, c, len);
    if (length) {
        *length = len - 1;
    }
    return c;
}

void JSCSSStyle::FinalizeCallback(JSObjectRef object) {
    JSCSSStyle *obj = static_cast<JSCSSStyle *>(JSObjectGetPrivate(object));
    delete obj;
}

JSValueRef JSCSSStyle::GetPropertyFunctionCallback(JSContextRef ctx,
                                                   JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                                                   const JSValueRef arguments[], JSValueRef *exception) {
    if (argumentCount > 1) {
        JSCSSStyle *obj = static_cast<JSCSSStyle *>(JSObjectGetPrivate(thisObject));
        JSStringRef propertyName = JSValueToStringCopy(ctx, arguments[0], exception);
        if (*exception) {
            return JSValueMakeUndefined(ctx);
        }
        char *name = CreateStringFromJSString(propertyName);
        JSStringRelease(propertyName);
        CSSStylePropertyValue *value;
        if (obj->getCSSStyleDeclaration()->getProperty(name, &value) && value) {
            if (value->isNumber()) {
                switch (value->unitType()) {
                    case CSS_NUMBER: {
                        return JSValueMakeNumber(ctx, value->toNumber());
                    }
                    case CSS_PX: {
                        JSStringRef string = JSStringCreateWithUTF8CString(value->toString());
                        return JSValueMakeString(ctx, string);
                    }
                    default:
                        break;
                }
            } else {
                JSStringRef valueStr = JSStringCreateWithUTF8CString(value->toString());
                return JSValueMakeString(ctx, valueStr);
            }
        }

        delete[] name;
    }

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSCSSStyle::SetPropertyValueFunctionCallback(JSContextRef ctx,
                                                        JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                                                        const JSValueRef arguments[], JSValueRef *exception) {
    if (argumentCount > 2) {
        JSCSSStyle *obj = static_cast<JSCSSStyle *>(JSObjectGetPrivate(thisObject));
        JSStringRef propertyName = JSValueToStringCopy(ctx, arguments[0], exception);
        if (*exception) {
            return JSValueMakeUndefined(ctx);
        }
        char *name = CreateStringFromJSString(propertyName);
        JSStringRelease(propertyName);

        JSValueRef value = arguments[1];
        if (JSValueIsNumber(ctx, value)) {
            double numberValue = JSValueToNumber(ctx, value, exception);
            if (*exception != nullptr) {
                obj->getCSSStyleDeclaration()->setProperty(name, numberValue);
            }
        } else if (JSValueIsString(ctx, value)) {
            JSStringRef valueJStr = JSValueToStringCopy(ctx, value, exception);
            if (*exception != nullptr) {
                char *valueStr = CreateStringFromJSString(valueJStr);
                obj->getCSSStyleDeclaration()->setProperty(name, valueStr);
                delete[] valueStr;
            }
            JSStringRelease(valueJStr);
        }

        delete[] name;
    }

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSCSSStyle::RemovePropertyFunctionCallback(JSContextRef ctx,
                                                      JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                                                      JSValueRef const *arguments, JSValueRef *exception) {

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSCSSStyle::GetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception) {
    JSCSSStyle *obj = static_cast<JSCSSStyle *>(JSObjectGetPrivate(object));
    if (JSStringIsEqualToUTF8CString(propertyName, "cssText")) {
        JSStringRef string = JSStringCreateWithUTF8CString(obj->getCSSStyleDeclaration()->cssStyleText());
        return JSValueMakeString(ctx, string);
    } else if (JSStringIsEqualToUTF8CString(propertyName, "setProperty")) {
        if (!obj->m_setProperty) {
            JSStringRef functionName = JSStringCreateWithUTF8CString("setProperty");
            obj->m_setProperty = JSObjectMakeFunctionWithCallback(ctx, functionName, SetPropertyValueFunctionCallback);
//            JSValueProtect(ctx, obj->m_setProperty);
        }

        return obj->m_setProperty;

    } else if (JSStringIsEqualToUTF8CString(propertyName, "getPropertyValue")) {
        if (!obj->m_getPropertyValue) {
            JSStringRef functionName = JSStringCreateWithUTF8CString("getPropertyValue");
            obj->m_getPropertyValue = JSObjectMakeFunctionWithCallback(ctx, functionName, GetPropertyFunctionCallback);
//            JSValueProtect(ctx, obj->m_getPropertyValue);
        }

        return obj->m_getPropertyValue;

    } else if (JSStringIsEqualToUTF8CString(propertyName, "removeProperty")) {
        if (!obj->m_removeProperty) {
            JSStringRef functionName = JSStringCreateWithUTF8CString("removeProperty");
            obj->m_removeProperty = JSObjectMakeFunctionWithCallback(ctx, functionName, RemovePropertyFunctionCallback);
//            JSValueProtect(ctx, obj->m_removeProperty);
        }

        return obj->m_removeProperty;
    } else {
        char *c = CreateStringFromJSString(propertyName);
        CSSStylePropertyValue *value = nullptr;
        if (obj->getCSSStyleDeclaration()->getProperty(c, &value) && value) {
            if (value->isString() || (value->isNumber() && value->unitType() != CSS_NUMBER)) {
                JSStringRef stringValue = JSStringCreateWithUTF8CString(value->toString());
                return JSValueMakeString(ctx, stringValue);
            } else {
                return JSValueMakeNumber(ctx, value->toNumber());
            }
        }

        delete[] c;
    }

    return JSValueMakeUndefined(ctx);
}

void JSCSSStyle::GetPropertyNamesCallback(JSContextRef ctx, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames) {
    JSCSSStyle *obj = static_cast<JSCSSStyle *>(JSObjectGetPrivate(object));
    auto &properties = obj->getCSSStyleDeclaration()->properties();
    for (auto it : properties) {
        JSStringRef propertyName = JSStringCreateWithUTF8CString(it.first.c_str());
        JSPropertyNameAccumulatorAddName(propertyNames, propertyName);
    }
}

bool JSCSSStyle::SetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef *exception) {
    JSCSSStyle *obj = static_cast<JSCSSStyle *>(JSObjectGetPrivate(object));
    if (JSStringIsEqualToUTF8CString(propertyName, "cssText")) {
        JSStringRef stringValue = JSValueToStringCopy(ctx, value, exception);
        if (*exception) {
            return false;
        }
        char *valueStr = CreateStringFromJSString(stringValue);
        JSStringRelease(stringValue);
        obj->getCSSStyleDeclaration()->setCssStyleText(valueStr);
        delete[] valueStr;
        return true;

    } else {
        if (JSValueIsString(ctx, value)) {
            char *propertyStr = CreateStringFromJSString(propertyName);
            JSStringRef valueStr = JSValueToStringCopy(ctx, value, exception);
            if (*exception) {
                return false;
            }

            size_t strLength;
            char *str = CreateStringFromJSString(valueStr, &strLength);
            JSStringRelease(valueStr);
            if (strLength) {
                CSSToken token(str, 0, strLength - 1);
                auto propertyValue = CSSParser::tokenToCSSPropertyValue(token);
                if (propertyValue) {
                    obj->getCSSStyleDeclaration()->setProperty(propertyStr, *propertyValue);
                }
            } else {
                obj->getCSSStyleDeclaration()->removeProperty(propertyStr);
            }

            delete[] str;
            delete[] propertyStr;
            return true;
        } else if (JSValueIsNumber(ctx, value)) {
            char *propertyStr = CreateStringFromJSString(propertyName);
            CSSStylePropertyValue propertyValue(JSValueToNumber(ctx, value, exception));
            if (*exception) {
                return false;
            }

            obj->getCSSStyleDeclaration()->setProperty(propertyStr, propertyValue);

            delete[] propertyStr;
            return true;
        }

        return false;
    }
}

JSObjectRef JSCSSStyle::CreateCSSStyleDeclarationJSObject(JSContextRef context, std::unique_ptr<CSSStyleDeclaration> declaration) {
    static JSClassRef jsClass;
    if (!jsClass) {
        static JSClassDefinition cssStyleClassDefinition;
        cssStyleClassDefinition.className = "CSSStyleDeclaration";
        cssStyleClassDefinition.setProperty = SetPropertyCallback;
        cssStyleClassDefinition.getProperty = GetPropertyCallback;
        cssStyleClassDefinition.getPropertyNames = GetPropertyNamesCallback;
        cssStyleClassDefinition.finalize = FinalizeCallback;
        jsClass = JSClassCreate(&cssStyleClassDefinition);
    }

    JSCSSStyle *scriptInterface = new JSCSSStyle(std::move(declaration));
    // TODO: JSClassRelease(jsClass); ?
    return JSObjectMake(context, jsClass, scriptInterface);
}

JSCSSStyle::JSCSSStyle(std::unique_ptr<CSSStyleDeclaration> declaration) {
    m_styleDeclaration = std::move(declaration);
}

}