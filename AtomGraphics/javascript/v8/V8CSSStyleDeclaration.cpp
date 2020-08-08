//
// Created by neo on 2019/5/23.
//

#include <v8/v8.h>
#include "node/canvas/CSSParser.h"
#include "node/canvas/CSSToken.h"
#include "V8CSSStyleDeclaration.h"
#include "V8TemplateCache.h"
#include "V8TemplateConfiguration.h"
#include "V8Converter.h"

namespace AtomGraphics {

V8CSSStyleDeclaration::V8CSSStyleDeclaration(std::unique_ptr<CSSStyleDeclaration> declaration)
        : styleDeclaration_(std::move(declaration)) {

}

const WrapperTypeInfo V8CSSStyleDeclaration::wrapperTypeInfo = {
        domTemplate,
        "CSSStyleDeclaration"
};

const WrapperTypeInfo *V8CSSStyleDeclaration::getWrapperTypeInfo() const {
    return &wrapperTypeInfo;
}

void V8CSSStyleDeclaration::Trace(V8GCMarkingVisitor *visitor) {
    visitor->Visit(this);
}

v8::Local<v8::FunctionTemplate> V8CSSStyleDeclaration::domTemplate(v8::Isolate *isolate,
                                                                   v8::Local<v8::Context> context) {
    return V8TemplateCache::ClassTemplate(isolate, context, &wrapperTypeInfo, InstallTemplate);
}


void V8CSSStyleDeclaration::InstallTemplate(
        v8::Isolate *isolate,
        v8::Local<v8::Context> context,
        v8::Local<v8::FunctionTemplate> interfaceTemplate) {
    // Initialize the interface object's template.
    V8TemplateConfiguration::InitializeInterfaceTemplate(isolate,
                                                         interfaceTemplate,
                                                         wrapperTypeInfo.interface_name,
                                                         kV8DefaultWrapperInternalFieldCount);

    v8::Local<v8::ObjectTemplate> instanceTemplate = interfaceTemplate->InstanceTemplate();


    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "cssText", v8::NewStringType::kInternalized).ToLocalChecked(),
            GetCSSText, SetCSSText);

    instanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "getPropertyValue", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::FunctionTemplate::New(isolate, GetPropertyValue));

    instanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "setProperty", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::FunctionTemplate::New(isolate, SetProperty));

    instanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "removeProperty", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::FunctionTemplate::New(isolate, RemoveProperty));

    instanceTemplate->SetHandler(
            v8::NamedPropertyHandlerConfiguration(NamedPropertyGetterCallback,
                                                  NamedPropertySetterCallback));
}

void V8CSSStyleDeclaration::GetCSSText(v8::Local<v8::String> name,
                                       const v8::PropertyCallbackInfo<v8::Value> &info) {
    CSSStyleDeclaration *declaration = ToImpl(info.Holder());
    info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(),
                                                      declaration->cssStyleText(),
                                                      v8::NewStringType::kInternalized).ToLocalChecked());
}

void V8CSSStyleDeclaration::SetCSSText(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                                       const v8::PropertyCallbackInfo<void> &info) {
    v8::Isolate *isolate = info.GetIsolate();
    v8::String::Utf8Value utf8_value(isolate, value);
    CSSStyleDeclaration *declaration = ToImpl(info.Holder());
    declaration->setCssStyleText(*utf8_value);
}

void V8CSSStyleDeclaration::GetPropertyValue(const v8::FunctionCallbackInfo<v8::Value> &info) {
    if (info.kArgsLength == 0) {
        return;
    }

    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::String> propertyName;
    if (info[0]->ToString(info.Holder()->CreationContext()).ToLocal(&propertyName)) {
        v8::String::Utf8Value property_name_utf8_value(isolate, propertyName);
        CSSStyleDeclaration *declaration = ToImpl(info.Holder());
        CSSStylePropertyValue *value;
        if (declaration->getProperty(*property_name_utf8_value, &value) && value) {
            if (value->isNumber()) {
                switch (value->unitType()) {
                    case CSS_NUMBER:
                        info.GetReturnValue().Set(v8::Number::New(isolate, value->toNumber()));
                        break;
                    case CSS_PX:
                        info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value->toString(),
                                                                          v8::NewStringType::kInternalized).ToLocalChecked());
                        break;
                    default:
                        break;
                }
            } else {
                info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value->toString(),
                                                                  v8::NewStringType::kInternalized).ToLocalChecked());
            }
        }
    }
}

void V8CSSStyleDeclaration::SetProperty(const v8::FunctionCallbackInfo<v8::Value> &info) {
    if (info.kArgsLength != 2) {
        return;
    }

    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::String> propertyName;
    if (info[0]->ToString(info.Holder()->CreationContext()).ToLocal(&propertyName)) {
        v8::String::Utf8Value property_name_utf8_value(isolate, propertyName);
        v8::Local<v8::Value> propertyValue = info[1];
        CSSStyleDeclaration *declaration = ToImpl(info.Holder());
        if (propertyValue->IsNumber()) {
            declaration->setProperty(*property_name_utf8_value, propertyValue->ToNumber(isolate)->Value());
        } else if (propertyValue->IsString()) {
            v8::Local<v8::String> propertyStringValue;
            if (propertyValue->ToString(info.Holder()->CreationContext()).ToLocal(&propertyStringValue)) {
                v8::String::Utf8Value property_utf8_value(isolate, propertyStringValue);
                declaration->setProperty(*property_name_utf8_value, *property_utf8_value);
            }
        }
    }
}

void V8CSSStyleDeclaration::RemoveProperty(const v8::FunctionCallbackInfo<v8::Value> &info) {
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::String> propertyName;
    if (info[0]->ToString(info.Holder()->CreationContext()).ToLocal(&propertyName)) {
        v8::String::Utf8Value utf8_value(isolate, propertyName);
        CSSStyleDeclaration *declaration = ToImpl(info.Holder());
        declaration->removeProperty(*utf8_value);
    }
}

void V8CSSStyleDeclaration::NamedPropertyGetterCallback(v8::Local<v8::Name> property,
                                                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    CSSStyleDeclaration *declaration = ToImpl(info.Holder());
    v8::Isolate *isolate = info.GetIsolate();
    std::string propertyName;
    ConvertFromV8(isolate, property, &propertyName);

    CSSStylePropertyValue *value = nullptr;
    if (declaration->getProperty(propertyName, &value) && value) {
        if (value->isString() || (value->isNumber() && value->unitType() != CSS_NUMBER)) {
            info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, value->toString(),
                                                              v8::NewStringType::kInternalized).ToLocalChecked());
        } else {
            info.GetReturnValue().Set(v8::Number::New(isolate, value->toNumber()));
        }
    }
}

void V8CSSStyleDeclaration::NamedPropertySetterCallback(v8::Local<v8::Name> property,
                                                        v8::Local<v8::Value> value,
                                                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    v8::Isolate *isolate = info.GetIsolate();
    CSSStyleDeclaration *declaration = ToImpl(info.Holder());
    std::string propertyName;
    ConvertFromV8(isolate, property, &propertyName);
    if (value->IsString()) {
        v8::String::Utf8Value utf8_value(isolate, value);
        int strLength = utf8_value.length();
        if (strLength) {
            CSSToken token(*utf8_value, 0, strLength - 1);
            auto propertyValue = CSSParser::tokenToCSSPropertyValue(token);
            if (propertyValue) {
                declaration->setProperty(propertyName, *propertyValue);
            }
        } else {
            declaration->removeProperty(propertyName);
        }

    } else if (value->IsNumber()) {
        v8::Local<v8::Number> numberValue = value->ToNumber(info.Holder()->CreationContext()).ToLocalChecked();
        if (!numberValue.IsEmpty()) {
            declaration->setProperty(propertyName, CSSStylePropertyValue(numberValue->Value()));
        }
    }
}

}