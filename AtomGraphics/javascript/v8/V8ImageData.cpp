//
// Created by neo on 2019/1/30.
//

#include "V8ImageData.h"
#include "V8TemplateCache.h"
#include "V8TemplateConfiguration.h"
#include "V8Constructor.h"
#include "V8DataConvertor.h"
#include "V8Platform.h"
#include "V8GCScriptWrappableHeap.h"

using namespace v8;

namespace AtomGraphics {

V8ImageData::V8ImageData(scoped_refptr<ImageData> imageData)
        : ScriptWrappable(),
          m_imageData(std::move(imageData)) {
    JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(m_imageData->bytesCount());
}

V8ImageData::~V8ImageData() {
    JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(-m_imageData->bytesCount());
    m_data.Reset();
}

const WrapperTypeInfo V8ImageData::wrapperTypeInfo = {
        domTemplate,
        "ImageData"
};

void V8ImageData::Trace(V8GCMarkingVisitor *visitor) {
    //visit data_
    visitor->Visit(this);
}

const WrapperTypeInfo *V8ImageData::getWrapperTypeInfo() const {
    return &wrapperTypeInfo;
}

void V8ImageData::constructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    if (!info.IsConstructCall()) {
        return;
    }

    if (ConstructorMode::Current(info.GetIsolate()) == ConstructorMode::kWrapExistingObject) {
        info.GetReturnValue().Set(info.Holder());
        return;
    }

    V8ImageData::constructor(info);
}

void V8ImageData::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
    int argsLength = info.Length();
    if (argsLength == 2) {
        if (info[0]->IsNumber()) {
            uint32_t sw;
            uint32_t sh;
            sw = ToUInt32(info.GetIsolate(), info[0], kNormalConversion);
            sh = ToUInt32(info.GetIsolate(), info[1], kNormalConversion);
            auto imageData = ImageData::Create(
                    IntSize(static_cast<size_t >(sw), static_cast<size_t >(sh)),
                    4 * sw);
            V8ImageData *scriptWrapper = new V8ImageData(std::move(imageData));
            v8::Local<v8::Object> wrapper = info.Holder();
            wrapper = scriptWrapper->AssociateWithWrapper(info.GetIsolate(),
                                                          &wrapperTypeInfo,
                                                          wrapper);

            info.GetReturnValue().Set(wrapper);
        }
    } else if (argsLength >= 3) {//Uint8ClampedArray, width [, height]

    }

}


v8::Local<v8::FunctionTemplate> V8ImageData::domTemplate(v8::Isolate *isolate,
                                                         Local<Context> context) {
    return V8TemplateCache::ClassTemplate(isolate, context, &wrapperTypeInfo, InstallTemplate);
}

void V8ImageData::InstallTemplate(
        v8::Isolate *isolate,
        Local<Context> context,
        v8::Local<v8::FunctionTemplate> interfaceTemplate) {
    // Initialize the interface object's template.
    V8TemplateConfiguration::InitializeInterfaceTemplate(isolate, interfaceTemplate,
                                                         wrapperTypeInfo.interface_name,
                                                         kV8DefaultWrapperInternalFieldCount);
    interfaceTemplate->SetCallHandler(V8ImageData::constructorCallback);
    interfaceTemplate->SetLength(2);

    v8::Local<v8::ObjectTemplate> instanceTemplate = interfaceTemplate->InstanceTemplate();

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "data", NewStringType::kInternalized).ToLocalChecked(),
            GetData);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "width", NewStringType::kInternalized).ToLocalChecked(),
            GetWidth);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "height", NewStringType::kInternalized).ToLocalChecked(),
            GetHeight);
}


void V8ImageData::GetData(Local<v8::String> name, const PropertyCallbackInfo<Value> &info) {
    V8ImageData *v8ImageData = dynamic_cast<V8ImageData *>(ToScriptWrappable(info.Holder()));
    v8::Isolate *isolate = info.GetIsolate();
    if (v8ImageData->m_data.IsEmpty()) {
        v8::Local<v8::ArrayBuffer> arrayBuffer = v8::ArrayBuffer::New(isolate,
                                                                      v8ImageData->m_imageData->data(),
                                                                      v8ImageData->m_imageData->bytesCount());
        Local<Uint8ClampedArray> data = v8::Uint8ClampedArray::New(arrayBuffer, 0, arrayBuffer->ByteLength());
        v8ImageData->m_data.Reset(isolate, data);
        info.GetReturnValue().Set(data);
    } else {
        info.GetReturnValue().Set(v8ImageData->m_data.Get(isolate));
    }

}

void V8ImageData::GetWidth(Local<v8::String> name, const PropertyCallbackInfo<Value> &info) {
    V8ImageData *v8ImageData = dynamic_cast<V8ImageData *>(ToScriptWrappable(info.Holder()));
    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), v8ImageData->m_imageData->width()));
}

void V8ImageData::GetHeight(Local<v8::String> name, const PropertyCallbackInfo<Value> &info) {
    V8ImageData *v8ImageData = dynamic_cast<V8ImageData *>(ToScriptWrappable(info.Holder()));
    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), v8ImageData->m_imageData->height()));
}

}