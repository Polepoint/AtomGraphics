//
// Created by neo on 2018/10/16.
//

#include <node/canvas/CSSParser.h>
#include "V8CanvasRenderingContext.h"
#include "V8Canvas.h"
#include "V8ImageData.h"
#include "V8CanvasStyle.h"
#include "V8TemplateCache.h"
#include "V8TemplateConfiguration.h"
#include "V8Image.h"

using namespace v8;

namespace AtomGraphics {

V8CanvasRenderingContext::V8CanvasRenderingContext(
        std::shared_ptr<CanvasRenderingContext2D> context)
        : m_context(std::move(context)) {

}

V8CanvasRenderingContext::~V8CanvasRenderingContext() {

}

const WrapperTypeInfo V8CanvasRenderingContext::wrapperTypeInfo = {
        domTemplate,
        "CanvasRenderingContext"
};

const WrapperTypeInfo *V8CanvasRenderingContext::getWrapperTypeInfo() const {
    return &wrapperTypeInfo;
}

v8::Local<v8::FunctionTemplate> V8CanvasRenderingContext::domTemplate(v8::Isolate *isolate,
                                                                      v8::Local<v8::Context> context) {
    return V8TemplateCache::ClassTemplate(isolate, context, &wrapperTypeInfo, InstallTemplate);
}

void V8CanvasRenderingContext::InstallTemplate(
        v8::Isolate *isolate,
        Local<Context> context,
        v8::Local<v8::FunctionTemplate> interfaceTemplate) {
    // Initialize the interface object's template.
    V8TemplateConfiguration::InitializeInterfaceTemplate(isolate,
                                                         interfaceTemplate,
                                                         wrapperTypeInfo.interface_name,
                                                         kV8DefaultWrapperInternalFieldCount);

    v8::Local<v8::ObjectTemplate> InstanceTemplate = interfaceTemplate->InstanceTemplate();

    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "fillStyle",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetFillStyle, SetFillStyle);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "strokeStyle",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetStrokeStyle, SetStrokeStyle);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "shadowColor",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetShadowColor, SetShadowColor);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "shadowBlur",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetShadowBlur, SetShadowBlur);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "shadowOffsetX",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetShadowOffsetX, SetShadowOffsetX);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "shadowOffsetY",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetShadowOffsetY, SetShadowOffsetY);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "lineWidth",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetLineWidth, SetLineWidth);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "lineCap",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetLineCap, SetLineCap);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "lineJoin",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetLineJoin, SetLineJoin);

    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "lineDashOffset",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetLineDashOffset, SetLineDashOffset);

    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "miterLimit",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetMiterLimit, SetMiterLimit);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "globalAlpha",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetGlobalAlpha, SetGlobalAlpha);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "font", v8::NewStringType::kNormal).ToLocalChecked(),
            GetFont, SetFont);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "textAlign",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetTextAlign, SetTextAlign);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "textBaseline",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetTextBaseline, SetTextBaseline);
    InstanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "globalCompositeOperation",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            GetGlobalCompositeOperation, SetGlobalCompositeOperation);


    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "beginPath",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, BeginPath));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "stroke", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Stroke));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "fill", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Fill));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "moveTo", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, MoveTo));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "closePath",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, ClosePath));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "lineTo", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, LineTo));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "clip", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Clip));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "quadraticCurveTo",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, QuadraticCurveTo));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "bezierCurveTo",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, BezierCurveTo));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "arc", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Arc));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "arcTo", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, ArcTo));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "isPointInPath",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, IsPointInPath));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "rect", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Rect));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "fillRect",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, FillRect));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "strokeRect",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, StrokeRect));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "clearRect",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, ClearRect));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "scale", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Scale));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "rotate", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Rotate));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "transform",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Transform));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "translate",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Translate));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "setTransform",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, SetTransform));

    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "setLineDash",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, SetLineDash));


    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "fillText",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, FillText));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "strokeText",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, StrokeText));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "measureText",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, MeasureText));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "createLinearGradient",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, CreateLinearGradient));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "createPattern",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, CreatePattern));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "createRadialGradient",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, CreateRadialGradient));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "drawImage",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, DrawImage));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "createImageData",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, CreateImageData));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "getImageData",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, GetImageData));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "putImageData",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, PutImageData));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "save", v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Save));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "restore",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, Restore));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "createEvent",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, CreateEvent));
    InstanceTemplate->Set(
            v8::String::NewFromUtf8(isolate, "toDataURL",
                                    v8::NewStringType::kNormal).ToLocalChecked(),
            FunctionTemplate::New(isolate, ToDataURL));
}

void V8CanvasRenderingContext::Trace(V8GCMarkingVisitor *visitor) {
    visitor->Visit(this);
}

void V8CanvasRenderingContext::GetFillStyle(Local<v8::String> name,
                                            const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetFillStyle(Local<v8::String> name, Local<Value> value,
                                            const PropertyCallbackInfo<void> &info) {
    CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
    if (value->IsUndefined() || value->IsNull()) {
        context2D->setStrokeStyle(Color4F::BLACK);
    } else if (value->IsString()) {
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        Color4F colorStyle = CSSParser::parseCSSColor(*utf8_value);
        context2D->setFillStyle(colorStyle);
    } else if (value->IsObject()) {
        v8::Isolate *isolate = info.GetIsolate();
        if (V8CanvasStyle::hasInstance(value, isolate)) {
            CanvasStyle *style = V8CanvasStyle::ToImpl(value->ToObject());
            context2D->setFillStyle(*style);
        }
    }
}

void V8CanvasRenderingContext::GetStrokeStyle(Local<v8::String> name,
                                              const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetStrokeStyle(Local<v8::String> name, Local<Value> value,
                                              const PropertyCallbackInfo<void> &info) {
    CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
    if (value->IsUndefined() || value->IsNull()) {
        context2D->setStrokeStyle(Color4F::BLACK);
    } else if (value->IsString()) {
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        Color4F colorStyle = CSSParser::parseCSSColor(*utf8_value);
        context2D->setStrokeStyle(colorStyle);
    } else if (value->IsObject()) {
        v8::Isolate *isolate = info.GetIsolate();
        if (V8CanvasStyle::hasInstance(value, isolate)) {
            CanvasStyle *style = V8CanvasStyle::ToImpl(value->ToObject());
            context2D->setStrokeStyle(*style);
        }
    }
}

void V8CanvasRenderingContext::GetShadowColor(Local<v8::String> name,
                                              const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetShadowColor(Local<v8::String> name, Local<Value> value,
                                              const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        Color4F colorStyle = CSSParser::parseCSSColor(*utf8_value);
        context2D->setShadowColor(colorStyle);
    }
}

void V8CanvasRenderingContext::GetShadowBlur(Local<v8::String> name,
                                             const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetShadowBlur(Local<v8::String> name, Local<Value> value,
                                             const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setShadowBlur(static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetShadowOffsetX(Local<v8::String> name,
                                                const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetShadowOffsetX(Local<v8::String> name, Local<Value> value,
                                                const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setShadowOffsetX(
                static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetShadowOffsetY(Local<v8::String> name,
                                                const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetShadowOffsetY(Local<v8::String> name, Local<Value> value,
                                                const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setShadowOffsetY(
                static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetLineWidth(Local<v8::String> name,
                                            const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetLineWidth(Local<v8::String> name, Local<Value> value,
                                            const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setLineWidth(static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetLineCap(Local<v8::String> name,
                                          const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetLineCap(Local<v8::String> name, Local<Value> value,
                                          const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        LineCap lineCap;
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        if (parseLineCap(*utf8_value, lineCap)) {
            context2D->setLineCap(lineCap);
        }
    }
}

void V8CanvasRenderingContext::GetLineJoin(Local<v8::String> name,
                                           const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetLineJoin(Local<v8::String> name, Local<Value> value,
                                           const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        LineJoin lineJoin;
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        if (parseLineJoin(*utf8_value, lineJoin)) {
            context2D->setLineJoin(lineJoin);
        }
    }
}

void V8CanvasRenderingContext::GetLineDashOffset(Local<v8::String> name,
                                                 const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetLineDashOffset(Local<v8::String> name, Local<Value> value,
                                                 const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setLineDashOffset(
                static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetMiterLimit(Local<v8::String> name,
                                             const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetMiterLimit(Local<v8::String> name, Local<Value> value,
                                             const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setMiterLimit(static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetGlobalAlpha(Local<v8::String> name,
                                              const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetGlobalAlpha(Local<v8::String> name, Local<Value> value,
                                              const PropertyCallbackInfo<void> &info) {
    if (value->IsNumber()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        context2D->setGlobalAlpha(static_cast<float>(value->ToNumber(info.GetIsolate())->Value()));
    }
}

void V8CanvasRenderingContext::GetGlobalCompositeOperation(Local<v8::String> name,
                                                           const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetGlobalCompositeOperation(Local<v8::String> name,
                                                           Local<Value> value,
                                                           const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        CompositeOperator compositeOperator;
        BlendMode blendMode;
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        if (parseCompositeAndBlendOperator(*utf8_value, compositeOperator, blendMode)) {
            context2D->setGlobalCompositeOperation(compositeOperator, blendMode);
        }
    }
}

void V8CanvasRenderingContext::GetFont(Local<v8::String> name,
                                       const PropertyCallbackInfo<Value> &info) {

    CanvasRenderingContext2D *context2d = ToImpl(info.Holder());
    info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(),
                                                      context2d->getFont().data(),
                                                      v8::NewStringType::kInternalized).ToLocalChecked());
}

void V8CanvasRenderingContext::SetFont(Local<v8::String> name, Local<Value> value,
                                       const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        context2D->setFont(*utf8_value);
    }
}

void V8CanvasRenderingContext::GetTextAlign(Local<v8::String> name,
                                            const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetTextAlign(Local<v8::String> name, Local<Value> value,
                                            const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        TextAlign textAlign;
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        if (parseTextAlign(*utf8_value, textAlign)) {
            context2D->setTextAlign(textAlign);
        }
    }
}

void V8CanvasRenderingContext::GetTextBaseline(Local<v8::String> name,
                                               const PropertyCallbackInfo<Value> &info) {

}

void V8CanvasRenderingContext::SetTextBaseline(Local<v8::String> name, Local<Value> value,
                                               const PropertyCallbackInfo<void> &info) {
    if (value->IsString()) {
        TextBaseline baseline;
        CanvasRenderingContext2D *context2D = ToImpl(info.Holder());
        v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
        if (parseTextBaseline(*utf8_value, baseline)) {
            context2D->setTextBaseline(baseline);
        }
    }
}

void V8CanvasRenderingContext::BeginPath(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->beginPath();
}

void V8CanvasRenderingContext::Stroke(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->stroke();
}

void V8CanvasRenderingContext::Fill(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->fill();
}

void V8CanvasRenderingContext::MoveTo(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->moveTo(static_cast<float>(args[0]->NumberValue()),
                      static_cast<float>(args[1]->NumberValue()));
}

void V8CanvasRenderingContext::ClosePath(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->closePath();
}

void V8CanvasRenderingContext::LineTo(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->lineTo(static_cast<float>(args[0]->NumberValue()),
                      static_cast<float>(args[1]->NumberValue()));
}

void V8CanvasRenderingContext::Clip(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->clip();
}

void V8CanvasRenderingContext::QuadraticCurveTo(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->quadraticCurveTo(static_cast<float>(args[0]->NumberValue()),
                                static_cast<float>(args[1]->NumberValue()),
                                static_cast<float>(args[2]->NumberValue()),
                                static_cast<float>(args[3]->NumberValue()));
}

void V8CanvasRenderingContext::BezierCurveTo(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->bezierCurveTo(static_cast<float>(args[0]->NumberValue()),
                             static_cast<float>(args[1]->NumberValue()),
                             static_cast<float>(args[2]->NumberValue()),
                             static_cast<float>(args[3]->NumberValue()),
                             static_cast<float>(args[4]->NumberValue()),
                             static_cast<float>(args[5]->NumberValue()));
}

void V8CanvasRenderingContext::Arc(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->arc(static_cast<float>(args[0]->NumberValue()),
                   static_cast<float>(args[1]->NumberValue()),
                   static_cast<float>(args[2]->NumberValue()),
                   static_cast<float>(args[3]->NumberValue()),
                   static_cast<float>(args[4]->NumberValue()),
                   args[5]->BooleanValue());
}

void V8CanvasRenderingContext::ArcTo(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->arcTo(static_cast<float>(args[0]->NumberValue()),
                     static_cast<float>(args[1]->NumberValue()),
                     static_cast<float>(args[2]->NumberValue()),
                     static_cast<float>(args[3]->NumberValue()),
                     static_cast<float>(args[4]->NumberValue()));
}

void V8CanvasRenderingContext::IsPointInPath(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    args.GetReturnValue().Set(context2D->isPointInPath(
            static_cast<float>(args[0]->NumberValue()),
            static_cast<float>(args[1]->NumberValue())));
}

void V8CanvasRenderingContext::Rect(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->rect(static_cast<float>(args[0]->NumberValue()),
                    static_cast<float>(args[1]->NumberValue()),
                    static_cast<float>(args[2]->NumberValue()),
                    static_cast<float>(args[3]->NumberValue()));
}

void V8CanvasRenderingContext::FillRect(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->fillRect(static_cast<float>(args[0]->NumberValue()),
                        static_cast<float>(args[1]->NumberValue()),
                        static_cast<float>(args[2]->NumberValue()),
                        static_cast<float>(args[3]->NumberValue()));
}

void V8CanvasRenderingContext::StrokeRect(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->strokeRect(static_cast<float>(args[0]->NumberValue()),
                          static_cast<float>(args[1]->NumberValue()),
                          static_cast<float>(args[2]->NumberValue()),
                          static_cast<float>(args[3]->NumberValue()));
}

void V8CanvasRenderingContext::ClearRect(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->clearRect(static_cast<float>(args[0]->NumberValue()),
                         static_cast<float>(args[1]->NumberValue()),
                         static_cast<float>(args[2]->NumberValue()),
                         static_cast<float>(args[3]->NumberValue()));
}

void V8CanvasRenderingContext::Scale(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->scale(static_cast<float>(args[0]->NumberValue()),
                     static_cast<float>(args[1]->NumberValue()));
}

void V8CanvasRenderingContext::Rotate(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->rotate(static_cast<float>(args[0]->NumberValue()));
}

void V8CanvasRenderingContext::Transform(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->transform(static_cast<float>(args[0]->NumberValue()),
                         static_cast<float>(args[1]->NumberValue()),
                         static_cast<float>(args[2]->NumberValue()),
                         static_cast<float>(args[3]->NumberValue()),
                         static_cast<float>(args[4]->NumberValue()),
                         static_cast<float>(args[5]->NumberValue()));
}

void V8CanvasRenderingContext::Translate(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->translate(static_cast<float>(args[0]->NumberValue()),
                         static_cast<float>(args[1]->NumberValue()));
}

void V8CanvasRenderingContext::SetTransform(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->setTransform(static_cast<float>(args[0]->NumberValue()),
                            static_cast<float>(args[1]->NumberValue()),
                            static_cast<float>(args[2]->NumberValue()),
                            static_cast<float>(args[3]->NumberValue()),
                            static_cast<float>(args[4]->NumberValue()),
                            static_cast<float>(args[5]->NumberValue()));
}

static void ConvertSequence(v8::Isolate *isolate,
                            v8::Local<v8::Array> v8_array,
                            std::vector<float> &result) {
    const uint32_t length = v8_array->Length();
    result.reserve(length);
    v8::TryCatch block(isolate);
    Local<Context> context = isolate->GetCurrentContext();
    for (uint32_t i = 0; i < length; ++i) {
        v8::Local<v8::Value> element;
        if (!v8_array->Get(context, i).ToLocal(&element)) {
            return;
        }
        double doubleValue = 0;
        if (!element->NumberValue(context).To(&doubleValue)) {
            return;
        }

        result.push_back(static_cast<float>(doubleValue));
    }
}

void V8CanvasRenderingContext::SetLineDash(const v8::FunctionCallbackInfo<v8::Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    if (args.kArgsLength > 0 && args[0]->IsArray()) {
        Local<Value> value = args[0];
        v8::Local<v8::Array> v8_array = value.As<v8::Array>();
        std::vector<float> array;
        ConvertSequence(args.GetIsolate(), v8_array, array);
        context2D->setLineDash(array);
    }
}

void V8CanvasRenderingContext::FillText(const FunctionCallbackInfo<Value> &args) {

    if (args.Length() < 3) {
        return;
    }

    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    v8::String::Utf8Value utf8_value(args.GetIsolate(), args[0]);
    const char *buffer = *utf8_value;
    if (args.Length() == 3) {
        context2D->fillText(base::String(buffer, strlen(buffer)),
                            static_cast<float>(args[1]->NumberValue()),
                            static_cast<float>(args[2]->NumberValue()),
                            nullptr);
    } else {
        float maxWith = static_cast<float>(args[3]->NumberValue());
        context2D->fillText(base::String(buffer, strlen(buffer)),
                            static_cast<float>(args[1]->NumberValue()),
                            static_cast<float>(args[2]->NumberValue()),
                            &maxWith);
    }
}

void V8CanvasRenderingContext::StrokeText(const FunctionCallbackInfo<Value> &args) {
    if (args.Length() < 3) {
        return;
    }

    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    v8::String::Utf8Value utf8_value(args.GetIsolate(), args[0]);
    const char *buffer = *utf8_value;
    if (args.Length() == 3) {
        context2D->strokeText(base::String(buffer, strlen(buffer)),
                              static_cast<float>(args[1]->NumberValue()),
                              static_cast<float>(args[2]->NumberValue()),
                              nullptr);
    } else {
        float maxWith = static_cast<float>(args[3]->NumberValue());
        context2D->strokeText(base::String(buffer, strlen(buffer)),
                              static_cast<float>(args[1]->NumberValue()),
                              static_cast<float>(args[2]->NumberValue()),
                              &maxWith);
    }
}

void V8CanvasRenderingContext::MeasureText(const FunctionCallbackInfo<Value> &args) {
    if (args.Length() > 0) {
        v8::Local<v8::Value> p0 = args[0];
        if (!p0->IsString() && !p0->IsNumber()) {
            return;
        }
        CanvasRenderingContext2D *ctx = ToImpl(args.Holder());

        Isolate *isolate = args.GetIsolate();
        Local<Object> textMetrics = v8::Object::New(isolate);
        v8::String::Utf8Value utf8_value(isolate, p0);
        const char *str = *utf8_value;
        textMetrics->Set(
                v8::String::NewFromUtf8(isolate, "width",
                                        v8::NewStringType::kNormal).ToLocalChecked(),
                Number::New(isolate,
                            ctx->measureText(base::String(str, std::strlen(str))).getWidth()));
        args.GetReturnValue().Set(textMetrics);
    }
}

void V8CanvasRenderingContext::CreateLinearGradient(const FunctionCallbackInfo<Value> &args) {
    std::shared_ptr<Gradient> linearGradient = std::make_shared<Gradient>(
            static_cast<float>(args[0]->NumberValue()),
            static_cast<float>(args[1]->NumberValue()),
            static_cast<float>(args[2]->NumberValue()),
            static_cast<float>(args[3]->NumberValue()));
    std::unique_ptr<CanvasStyle> canvasStyle(new CanvasStyle(std::move(linearGradient)));
    V8CanvasStyle *style = new V8CanvasStyle(std::move(canvasStyle));
    args.GetReturnValue().Set(style->Wrap(args.GetIsolate(), args.Holder()->CreationContext()));
}

void V8CanvasRenderingContext::CreateRadialGradient(const FunctionCallbackInfo<Value> &args) {
    std::shared_ptr<Gradient> radialGradient = std::make_shared<Gradient>(
            static_cast<float>(args[0]->NumberValue()),
            static_cast<float>(args[1]->NumberValue()),
            static_cast<float>(args[2]->NumberValue()),
            static_cast<float>(args[3]->NumberValue()),
            static_cast<float>(args[4]->NumberValue()),
            static_cast<float>(args[5]->NumberValue()));
    std::unique_ptr<CanvasStyle> canvasStyle(new CanvasStyle(std::move(radialGradient)));
    V8CanvasStyle *style = new V8CanvasStyle(std::move(canvasStyle));
    args.GetReturnValue().Set(style->Wrap(args.GetIsolate(), args.Holder()->CreationContext()));
}

void V8CanvasRenderingContext::CreatePattern(const FunctionCallbackInfo<Value> &args) {
    std::shared_ptr<CanvasImageSource> imageSource;
    CanvasNode *canvas = V8Canvas::ToImpl(args[0]->ToObject());
    if (canvas) {
        imageSource.reset(new CanvasImageSource(canvas));
    } else {
        ImageBitmap *image = V8Image::ToImpl(args[0]->ToObject());
        if (image) {
            imageSource.reset(new CanvasImageSource(image));
        }
    }

    if (!imageSource) {
        return;
    }

    v8::String::Utf8Value pattern_utf8_value(args.GetIsolate(), args[1]);

    std::shared_ptr<Pattern> canvasPattern;

    bool repeatX = false;
    bool repeatY = false;
    if (CSSParser::parseRepetitionType(*pattern_utf8_value, repeatX, repeatY)) {
        canvasPattern = std::make_shared<Pattern>(imageSource.get(), repeatX, repeatY);
    } else {
        canvasPattern = std::make_shared<Pattern>();
    }

    std::unique_ptr<CanvasStyle> canvasStyle(new CanvasStyle(canvasPattern));
    V8CanvasStyle *style = new V8CanvasStyle(std::move(canvasStyle));
    args.GetReturnValue().Set(style->Wrap(args.GetIsolate(), args.Holder()->CreationContext()));
}

void V8CanvasRenderingContext::DrawImage(const FunctionCallbackInfo<Value> &args) {
    if (args.Length() >= 3) {
        CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
        std::unique_ptr<CanvasImageSource> source;
        CanvasNode *canvas = V8Canvas::ToImpl(args[0]->ToObject());
        if (canvas) {
            source.reset(new CanvasImageSource(canvas));
        } else {
            ImageBitmap *image = V8Image::ToImpl(args[0]->ToObject());
            source.reset(new CanvasImageSource(image));
        }

        if (!source) {
            return;
        }

        int argsLength = args.Length();
        Isolate *isolate = args.GetIsolate();
        if (argsLength == 3) {
            Local<Integer> xValue = args[1]->ToInteger(isolate);
            Local<Integer> yValue = args[2]->ToInteger(isolate);
            context2D->drawImage(source.get(), xValue->Value(), yValue->Value());
        } else if (argsLength == 5) {
            int64_t x = args[1]->ToInteger(isolate)->Value();
            int64_t y = args[2]->ToInteger(isolate)->Value();
            int64_t width = args[3]->ToInteger(isolate)->Value();
            int64_t height = args[4]->ToInteger(isolate)->Value();
            context2D->drawImage(source.get(), x, y, width, height);
        } else if (argsLength == 7) {
            int64_t sourceX = args[1]->ToInteger(isolate)->Value();
            int64_t sourceY = args[2]->ToInteger(isolate)->Value();
            int64_t sourceWith = args[3]->ToInteger(isolate)->Value();
            int64_t sourceHeight = args[4]->ToInteger(isolate)->Value();
            int64_t destX = args[5]->ToInteger(isolate)->Value();
            int64_t destY = args[6]->ToInteger(isolate)->Value();
            int64_t destWidth = args[7]->ToInteger(isolate)->Value();
            int64_t destHeight = args[8]->ToInteger(isolate)->Value();
            context2D->drawImage(source.get(),
                                 sourceX, sourceY, sourceWith, sourceHeight,
                                 destX, destY, destWidth, destHeight);
        }
    }
}

void V8CanvasRenderingContext::CreateImageData(const FunctionCallbackInfo<Value> &args) {
    int argsLength = args.Length();
    if (argsLength == 0) {
        return;
    }

    Isolate *isolate = args.GetIsolate();

    if (argsLength == 2) {
        if (args[0]->IsNumber() && args[1]->IsNumber()) {
            int width = static_cast<int>(args[0]->ToInteger(isolate)->Value());
            int height = static_cast<int>(args[1]->ToInteger(isolate)->Value());
            scoped_refptr<ImageData> imageData = ImageData::Create({width, height}, 4 * width);
            V8ImageData *v8ImageData = new V8ImageData(std::move(imageData));
            args.GetReturnValue().Set(v8ImageData->Wrap(isolate, args.Holder()->CreationContext()));
        }
    } else if (argsLength == 1) {
        if (args[0]->IsObject()) {
            ImageData *imageData = V8ImageData::ToImpl(v8::Local<v8::Object>::Cast(args[0]));
            if (imageData) {
                scoped_refptr<ImageData> newImageData = ImageData::Create(
                        imageData->size(),
                        4 * imageData->size().width);
                V8ImageData *v8ImageData = new V8ImageData(std::move(newImageData));
                args.GetReturnValue().Set(
                        v8ImageData->Wrap(isolate, args.Holder()->CreationContext()));
            }
        }
    }
}

void V8CanvasRenderingContext::GetImageData(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = args.GetIsolate();
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    V8ImageData *v8ImageData = new V8ImageData(std::move(context2D->getImageData(
            static_cast<int>(args[0]->NumberValue()),
            static_cast<int>(args[1]->NumberValue()),
            static_cast<int>(args[2]->NumberValue()),
            static_cast<int>(args[3]->NumberValue())
    )));
    args.GetReturnValue().Set(v8ImageData->Wrap(isolate, args.Holder()->CreationContext()));
}

void V8CanvasRenderingContext::PutImageData(const FunctionCallbackInfo<Value> &args) {
    int argsCount = args.Length();
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    if (argsCount == 3) {
        ImageData *imageData = V8ImageData::ToImpl(v8::Local<v8::Object>::Cast(args[0]));
        context2D->putImageData(
                imageData,
                static_cast<int>(args[1]->NumberValue()),
                static_cast<int>(args[2]->NumberValue()));

    } else if (argsCount == 7) {
        ImageData *imageData = V8ImageData::ToImpl(v8::Local<v8::Object>::Cast(args[0]));
        context2D->putImageData(
                imageData,
                static_cast<int>(args[1]->NumberValue()),
                static_cast<int>(args[2]->NumberValue()),
                static_cast<int>(args[3]->NumberValue()),
                static_cast<int>(args[4]->NumberValue()),
                static_cast<int>(args[5]->NumberValue()),
                static_cast<int>(args[6]->NumberValue()));
    }
}

void V8CanvasRenderingContext::Save(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->save();
}

void V8CanvasRenderingContext::Restore(const FunctionCallbackInfo<Value> &args) {
    CanvasRenderingContext2D *context2D = ToImpl(args.Holder());
    context2D->restore();
}

void V8CanvasRenderingContext::CreateEvent(const FunctionCallbackInfo<Value> &args) {

}

void V8CanvasRenderingContext::ToDataURL(const FunctionCallbackInfo<Value> &args) {

}

}