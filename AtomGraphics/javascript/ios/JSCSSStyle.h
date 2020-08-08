//
// Created by neo on 2018/12/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "CSSStyleDeclaration.h"

namespace AtomGraphics {

class JSCSSStyle {

public:
    
    static JSObjectRef CreateCSSStyleDeclarationJSObject(JSContextRef context, std::unique_ptr<CSSStyleDeclaration> declaration);

private:
    std::unique_ptr<CSSStyleDeclaration> m_styleDeclaration;
    
    JSObjectRef m_removeProperty;
    JSObjectRef m_setProperty;
    JSObjectRef m_getPropertyValue;
    
    JSCSSStyle(std::unique_ptr<CSSStyleDeclaration> declaration);
    
    CSSStyleDeclaration *getCSSStyleDeclaration() {
        return m_styleDeclaration.get();
    }
    
    static char *CreateStringFromJSString(JSStringRef jsString, size_t *length = nullptr);
    
    static void FinalizeCallback(JSObjectRef object);
    
    static JSValueRef GetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception);
    
    static JSValueRef GetPropertyFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, JSValueRef const *arguments, JSValueRef *exception);
    
    static JSValueRef SetPropertyValueFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, JSValueRef const *arguments, JSValueRef *exception);
    
    static JSValueRef RemovePropertyFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, JSValueRef const *arguments, JSValueRef *exception);
    
    static bool SetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef *exception);
    
    static void GetPropertyNamesCallback(JSContextRef ctx, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames);
    
};

}