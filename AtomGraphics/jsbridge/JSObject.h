//
// Created by neo on 2018/5/15.
//

#ifndef ATOMGRAPHICS_JSOBJECT_H
#define ATOMGRAPHICS_JSOBJECT_H

/**
 * TODO 需要进一步完善
 * JSContext对Object的引用都是强引用，缺乏弱引用，比如比如context的渐变属性对象，用过一次即可销毁
 * 考察JSRetainPtr的用法
 */

#include <string>
#include <JavaScriptCore/JavaScript.h>
#include <functional>
#include <map>
#include <vector>
#include "JSContext.h"

namespace AtomGraphicsJSC {

    class JSContext;

    class JSObject;

    typedef std::function<JSObject *(JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                                     JSObject **arguments)>
            JSFunctionObjectCallback;

    typedef std::function<void(JSContext *ctx, JSObject *thisObject, size_t argumentCount,
                               JSObject **arguments)>
            JSFunctionObjectCallbackWithoutReturn;

    typedef std::function<JSObject *(JSContext *ctx, const char *propertyName)>
            JSGetPropertyCallback;

    typedef std::function<void(JSContext *ctx, const char *propertyName, JSObject *value)>
            JSSetPropertyCallback;

    class JSObject {

    public:

        template<typename T>
        static JSObject *CreateJSObject(JSContext *ctx, T *nativeObject) {
            JSClassDefinition definition = kJSClassDefinitionEmpty;
            const char *name = typeid(T).name();
            definition.className = name;
            JSObject *object = new JSObject(ctx, nativeObject, definition);
            object->m_className = name;

            return object;
        }

        static size_t ConvertJSStringToString(JSStringRef jsString, char **result);

        static JSObject *
        ConvertJSValueRefToJSObject(JSContext *ctx, JSValueRef jsValue, JSValueRef *exception);

        static JSObject *getReferenceObject(const JSObjectRef jsObject);

        static JSObject *getCachedObject(void *object);

        /**
         * static method to create a JSObject, if the nativeObject was created before,
         * the return value will be the cached JSObject created
         * @param ctx           JSContext instance
         * @param nativeObject  a pointer to a native object
         * @param definition    class definition
         * @return              a JSObject instance reference to the given nativeObject
         */
        static JSObject *CreateObject(JSContext *ctx, void *nativeObject = nullptr,
                                       JSClassDefinition definition = kJSClassDefinitionEmpty);

        static JSObject *CreateObject(JSContext *ctx, void *nativeObject, JSSetPropertyCallback propertySetter,
                                       JSGetPropertyCallback propertyGetter,
                                       JSClassDefinition definition = kJSClassDefinitionEmpty);

        static JSObject *CreateObject(JSContext *ctx, const char *stringBuffer);

        static JSObject *CreateObject(JSContext *ctx, double numberValue);

        static JSObject *CreateObject(JSContext *ctx, JSObjectRef jsObjectRef);

        static JSObject *CreateObject(JSContext *ctx, JSValueRef jsValueRef);

        static JSObject *
        CreateObject(JSContext *ctx, std::string functionName, JSFunctionObjectCallback functionValue);

        static JSObject *CreateObject(JSContext *ctx, std::string functionName,
                                       JSFunctionObjectCallbackWithoutReturn functionValue);

        JSObject(JSContext *ctx, void *nativeObject = nullptr,
                 JSClassDefinition definition = kJSClassDefinitionEmpty);

        JSObject(JSContext *ctx, void *nativeObject, JSSetPropertyCallback propertySetter,
                 JSGetPropertyCallback propertyGetter,
                 JSClassDefinition definition = kJSClassDefinitionEmpty);

        JSObject(JSContext *ctx, const char *stringBuffer);

        JSObject(JSContext *ctx, double numberValue);

        JSObject(JSContext *ctx, JSObjectRef jsObjectRef);

        JSObject(JSContext *ctx, JSValueRef jsValueRef);

        JSObject(JSContext *ctx, std::string functionName, JSFunctionObjectCallback functionValue);

        JSObject(JSContext *ctx, std::string functionName,
                 JSFunctionObjectCallbackWithoutReturn functionValue);

        virtual ~JSObject();


        void setProperty(std::string propertyName, const JSObject *value);

        void setProperty(std::string propertyName, JSFunctionObjectCallback callback);

        void setProperty(std::string propertyName, JSFunctionObjectCallbackWithoutReturn callback);

        const JSObject *getProperty(std::string propertyName);

        const char *getClassName() const {
            return m_className;
        }

        JSObjectRef toJSObjectRef() const {
            return m_jsObjectRef;
        }

        JSValueRef toJSValueRef() const {
            return m_jsValueRef;
        }

        const char *toString() const {
            return m_stringBuffer;
        }

        void *toObjectPointer() const {
            return m_dataPointer;
        };

        const double toDouble() {
            return m_numberValue;
        }

        const bool toBoolean() {
            return m_numberValue != 0;
        }

        const JSObject *callAsFunction(JSObject *thisObject);

        const JSObject *
        callAsFunctionWithArguments(JSObject *thisObject, std::vector<JSObject *> arguments);

        bool isObject() const;

        bool isString() const;

        bool isNumber() const;

        bool isBoolean() const;

        bool isNull() const;

        bool isUndefined() const;

        bool isFunction() const;

    private:

        JSObjectRef m_jsObjectRef{nullptr};
        JSValueRef m_jsValueRef{nullptr};
        JSContext *m_context;
        const char *m_className;

        void *m_dataPointer{nullptr};
        double m_numberValue;
        char *m_stringBuffer{nullptr};
        JSFunctionObjectCallback m_function;

        JSSetPropertyCallback m_propertySetter;
        JSGetPropertyCallback m_propertyGetter;

        bool _isObject{false};
        bool _isString{false};
        bool _isNumber{false};
        bool _isUndefined{false};
        bool _isNull{false};
        bool _isBoolean{false};
        bool _isFunction{false};

        std::map<std::string, const JSObject *> m_propertyMap;

        static std::map<JSObjectRef, JSObject *> jObjMap;
        static std::map<long, JSObject *> objRetainMap;

        static inline JSValueRef
        JSCFunctionCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
                            size_t argumentCount, const JSValueRef arguments[],
                            JSValueRef *exception);

        static inline bool
        OnSetProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName,
                      JSValueRef value, JSValueRef *exception);

        static inline JSValueRef
        OnGetProperty(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName,
                      JSValueRef *exception);
    };
}


#endif //ATOMGRAPHICS_JSOBJECT_H
