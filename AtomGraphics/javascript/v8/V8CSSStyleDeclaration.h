//
// Created by neo on 2019/5/23.
//

#ifndef ATOMGRAPHICS_V8CSSSTYLE_H
#define ATOMGRAPHICS_V8CSSSTYLE_H

#include "node/canvas/CSSStyleDeclaration.h"
#include "ScriptWrappable.h"

namespace AtomGraphics {

class V8CSSStyleDeclaration : public ScriptWrappable {

public:

    V8CSSStyleDeclaration(std::unique_ptr<CSSStyleDeclaration> declaration);

    static const WrapperTypeInfo wrapperTypeInfo;

    const WrapperTypeInfo *getWrapperTypeInfo() const override;

    static CSSStyleDeclaration *ToImpl(v8::Local<v8::Object> object) {
        return ToScriptWrappable(object)->ToImpl<V8CSSStyleDeclaration>()->styleDeclaration_.get();
    }

    void Trace(V8GCMarkingVisitor *visitor) override;


private:

    std::unique_ptr<CSSStyleDeclaration> styleDeclaration_;

    static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context);

    static void InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                v8::Local<v8::FunctionTemplate> interfaceTemplate);

    static void GetCSSText(v8::Local<v8::String> name,
                           const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetCSSText(v8::Local<v8::String> name, v8::Local<v8::Value>,
                           const v8::PropertyCallbackInfo<void> &info);

    static void GetPropertyValue(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void SetProperty(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void RemoveProperty(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void NamedPropertyGetterCallback(v8::Local<v8::Name> property,
                                            const v8::PropertyCallbackInfo<v8::Value> &info);

    static void NamedPropertySetterCallback(v8::Local<v8::Name> property,
                                            v8::Local<v8::Value> value,
                                            const v8::PropertyCallbackInfo<v8::Value> &info);
};

}


#endif //ATOMGRAPHICS_V8CSSSTYLE_H
