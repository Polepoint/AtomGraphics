//
// Created by neo on 2019/2/20.
//

#include "node/canvas/CSSParser.h"
#include "V8CanvasStyle.h"
#include "V8TemplateCache.h"
#include "V8TemplateConfiguration.h"

using namespace v8;

namespace AtomGraphics {

V8CanvasStyle::V8CanvasStyle(std::unique_ptr<CanvasStyle> style)
        : m_style(std::move(style)) {

}

const WrapperTypeInfo V8CanvasStyle::wrapperTypeInfo = {
        domTemplate,
        "CanvasStyle"
};

const WrapperTypeInfo *V8CanvasStyle::getWrapperTypeInfo() const {
    return &wrapperTypeInfo;
}

void V8CanvasStyle::Trace(V8GCMarkingVisitor *visitor) {
    visitor->Visit(this);
}

v8::Local<v8::FunctionTemplate> V8CanvasStyle::domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context) {
    return V8TemplateCache::ClassTemplate(isolate, context, &wrapperTypeInfo, InstallTemplate);
}

void V8CanvasStyle::InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                    v8::Local<v8::FunctionTemplate> interfaceTemplate) {
    // Initialize the interface object's template.
    V8TemplateConfiguration::InitializeInterfaceTemplate(isolate,
                                                         interfaceTemplate,
                                                         wrapperTypeInfo.interface_name,
                                                         kV8DefaultWrapperInternalFieldCount);

    v8::Local<v8::ObjectTemplate> instanceTemplate = interfaceTemplate->InstanceTemplate();

    instanceTemplate->Set(v8::String::NewFromUtf8(isolate, "addColorStop", v8::NewStringType::kNormal).ToLocalChecked(),
                          FunctionTemplate::New(isolate, AddColorStop));

}


void V8CanvasStyle::AddColorStop(const FunctionCallbackInfo<Value> &args) {
    CanvasStyle *style = ToImpl(args.Holder());
    if (!style->isGradientStyle()) {
        return;
    }

    v8::String::Utf8Value color_utf8_value(args.GetIsolate(), args[1]);
    Color4F stopColor = CSSParser::parseCSSColor(*color_utf8_value);

    style->getCanvasGradient()->addColorStop(static_cast<float>(args[0]->NumberValue()), stopColor);
}


}