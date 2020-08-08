//
// Created by neo on 2018/10/16.
//

#include <v8/v8.h>
#include "V8Canvas.h"
#include "V8CanvasRenderingContext.h"
#include "V8TemplateCache.h"
#include "V8TemplateConfiguration.h"
#include "V8Platform.h"
#include "V8Converter.h"
#include "node/canvas/CSSParser.h"

using namespace v8;

namespace AtomGraphics {

using namespace JS;

std::map<Node *, V8Canvas *> canvasWrappableCache;

V8Canvas *V8Canvas::CreateCanvasWrappableFromCache(std::shared_ptr<Node> node) {
    auto it = canvasWrappableCache.find(node.get());
    if (it != canvasWrappableCache.end()) {
        return it->second;
    }

    V8Canvas *canvas = new V8Canvas(node);
    canvasWrappableCache.insert(std::make_pair(node.get(), canvas));
    return canvas;
}

V8Canvas::V8Canvas(const std::shared_ptr<Node> &node)
        : m_node(node) {
    CanvasNode *canvasNode = dynamic_cast<CanvasNode *>(node.get());
    JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(
            static_cast<int64_t>(canvasNode->contentWidth() * canvasNode->contentHeight() * 4));
}

V8Canvas::~V8Canvas() {
    auto it = canvasWrappableCache.find(m_node.get());
    if (it != canvasWrappableCache.end()) {
        canvasWrappableCache.erase(it);
    }

    CanvasNode *canvasNode = dynamic_cast<CanvasNode *>(m_node.get());
    JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(
            -static_cast<int64_t>(canvasNode->contentWidth() * canvasNode->contentHeight() * 4));
}

const WrapperTypeInfo V8Canvas::wrapperTypeInfo = {
        domTemplate,
        "Canvas"
};

const WrapperTypeInfo *V8Canvas::getWrapperTypeInfo() const {
    return &wrapperTypeInfo;
}

void V8Canvas::Trace(V8GCMarkingVisitor *visitor) {
    if (context_) {
        visitor->Visit(context_);
    }
    if (style_) {
        visitor->Visit(style_);
    }
    visitor->Visit(this);
}

v8::Local<v8::FunctionTemplate> V8Canvas::domTemplate(v8::Isolate *isolate,
                                                      v8::Local<v8::Context> context) {
    return V8TemplateCache::ClassTemplate(isolate, context, &wrapperTypeInfo, InstallTemplate);
}

void V8Canvas::InstallTemplate(
        v8::Isolate *isolate,
        Local<Context> context,
        v8::Local<v8::FunctionTemplate> interfaceTemplate) {
    // Initialize the interface object's template.
    V8TemplateConfiguration::InitializeInterfaceTemplate(isolate,
                                                         interfaceTemplate,
                                                         wrapperTypeInfo.interface_name,
                                                         kV8DefaultWrapperInternalFieldCount);

    v8::Local<v8::ObjectTemplate> instanceTemplate = interfaceTemplate->InstanceTemplate();

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "width", NewStringType::kInternalized).ToLocalChecked(),
            GetWidth, SetWidth);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "height", NewStringType::kInternalized).ToLocalChecked(),
            GetHeight, SetHeight);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "clientWidth", NewStringType::kInternalized).ToLocalChecked(),
            GetClientWidth);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "clientHeight", NewStringType::kInternalized).ToLocalChecked(),
            GetClientHeight);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "style", NewStringType::kInternalized).ToLocalChecked(),
            GetStyle);

    instanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "getContext", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::FunctionTemplate::New(isolate, GetContext));

    instanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "setAttribute", v8::NewStringType::kNormal).ToLocalChecked(),
            v8::FunctionTemplate::New(isolate, SetAttribute));
}

void V8Canvas::GetWidth(Local<v8::String> name,
                        const PropertyCallbackInfo<Value> &info) {
    CanvasNode *canvas = ToImpl(info.Holder());
    float width = canvas->contentWidth();
    info.GetReturnValue().Set(Number::New(info.GetIsolate(), width));
}

void V8Canvas::SetWidth(Local<v8::String> name, Local<Value> value,
                        const PropertyCallbackInfo<void> &info) {
    CanvasNode *canvas = ToImpl(info.Holder());
    Local<Number> widthValue = Local<Integer>::Cast(value);
    float oldWidth = canvas->contentWidth();
    float newWidth = static_cast<int >(widthValue->Value());
    canvas->setContentWidth(newWidth);
    JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(
            static_cast<int64_t>((newWidth - oldWidth) * canvas->contentHeight() * 4));

}

void V8Canvas::GetHeight(Local<v8::String> name,
                         const PropertyCallbackInfo<Value> &info) {
    CanvasNode *canvas = ToImpl(info.Holder());
    float height = canvas->contentHeight();
    info.GetReturnValue().Set(Number::New(info.GetIsolate(), height));
}

void V8Canvas::SetHeight(Local<v8::String> name, Local<Value> value,
                         const PropertyCallbackInfo<void> &info) {
    CanvasNode *canvas = ToImpl(info.Holder());
    Local<Number> heightValue = Local<Integer>::Cast(value);
    float oldHeight = canvas->contentHeight();
    float newHeight = static_cast<int >(heightValue->Value());
    canvas->setContentHeight(newHeight);
    JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(
            static_cast<int64_t>((newHeight - oldHeight) * canvas->contentWidth() * 4));
}

void V8Canvas::GetClientWidth(v8::Local<v8::String> name,
                              const v8::PropertyCallbackInfo<v8::Value> &info) {
    CanvasNode *canvas = ToImpl(info.Holder());
    float width = canvas->getFrame().size.width;
    info.GetReturnValue().Set(Number::New(info.GetIsolate(), width));
}

void V8Canvas::GetClientHeight(v8::Local<v8::String> name,
                               const v8::PropertyCallbackInfo<v8::Value> &info) {
    CanvasNode *canvas = ToImpl(info.Holder());
    float height = canvas->getFrame().size.height;
    info.GetReturnValue().Set(Number::New(info.GetIsolate(), height));
}

void ChangeStyleProperty(const char *propertyName, const CSSStylePropertyValue &value, void *userData) {
    CanvasNode *node = static_cast<CanvasNode *>(userData);
    if (std::strcmp(propertyName, "width") == 0) {
        auto rect = node->getFrame();
        rect.setWidth(static_cast<int>(value.toNumber()));
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "height") == 0) {
        auto rect = node->getFrame();
        rect.setHeight(static_cast<int>(value.toNumber()));
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "left") == 0) {
        auto rect = node->getFrame();
        float maxX = rect.getMaxX();
        rect.origin.x = static_cast<int>(value.toNumber());
        rect.setWidth(maxX - rect.origin.x);
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "top") == 0) {
        auto rect = node->getFrame();
        float maxY = rect.getMaxY();
        rect.origin.y = static_cast<int>(value.toNumber());
        rect.setHeight(maxY - rect.origin.y);
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "right") == 0) {
        auto rect = node->getFrame();
        rect.setWidth(static_cast<int>(value.toNumber() - rect.origin.x));
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "bottom") == 0) {
        auto rect = node->getFrame();
        rect.setHeight(static_cast<int>(value.toNumber() - rect.origin.y));
        node->setFrame(rect);
    }
}

void V8Canvas::GetStyle(v8::Local<v8::String> name,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    V8Canvas *canvasWrappable = dynamic_cast<V8Canvas *>(ToScriptWrappable(info.Holder()));
    if (!canvasWrappable->style_) {
        CSSStyleDeclaration *declaration = new CSSStyleDeclaration(ChangeStyleProperty,
                                                                   canvasWrappable->m_node.get());
        CanvasNode *canvas = dynamic_cast<CanvasNode *>(canvasWrappable->m_node.get());
        declaration->setProperty("width", CSSStylePropertyValue(canvas->getFrame().width(), CSS_PX));
        declaration->setProperty("height", CSSStylePropertyValue(canvas->getFrame().height(), CSS_PX));
        canvasWrappable->style_ = new V8CSSStyleDeclaration(std::unique_ptr<CSSStyleDeclaration>(declaration));
    }
    info.GetReturnValue().Set(canvasWrappable->style_->Wrap(info.GetIsolate(), info.Holder()->CreationContext()));
}

void V8Canvas::GetContext(const v8::FunctionCallbackInfo<v8::Value> &args) {
    V8Canvas *canvasWrappable = dynamic_cast<V8Canvas *>(ToScriptWrappable(args.Holder()));

    Isolate *isolate = args.GetIsolate();
    if (!canvasWrappable->context_ ||
            canvasWrappable->context_->m_context != canvasWrappable->node()->getContext2d()) {
        canvasWrappable->context_ = new V8CanvasRenderingContext(canvasWrappable->node()->getContext2d());
    }
    args.GetReturnValue().Set(canvasWrappable->context_->Wrap(isolate, args.Holder()->CreationContext()));
}

void V8Canvas::SetAttribute(const v8::FunctionCallbackInfo<v8::Value> &args) {

    if (args.kArgsLength < 2) {
        return;
    }

    v8::Local<v8::Value> name = args[0];
    if (!name->IsString()) {
        return;
    }
    v8::Local<v8::Value> value = args[1];

    CanvasNode *canvas = ToImpl(args.Holder());
    Isolate *isolate = args.GetIsolate();
    std::string propertyName;
    ConvertFromV8(isolate, name, &propertyName);
    if (value->IsString()) {
        v8::String::Utf8Value utf8_value(isolate, value);
        int strLength = utf8_value.length();
        if (strLength) {
            CSSToken token(*utf8_value, 0, strLength - 1);
            auto propertyValue = CSSParser::tokenToCSSPropertyValue(token);
            if (propertyValue) {
                ChangeStyleProperty(propertyName.c_str(), *(propertyValue.get()), canvas);
            }
        }
    } else if (value->IsNumber()) {
        v8::Local<v8::Number> numberValue = value->ToNumber(args.Holder()->CreationContext())
                .ToLocalChecked();
        if (!numberValue.IsEmpty()) {
            ChangeStyleProperty(propertyName.c_str(),
                                CSSStylePropertyValue(numberValue->Value()),
                                canvas);
        }
    }

}

}
