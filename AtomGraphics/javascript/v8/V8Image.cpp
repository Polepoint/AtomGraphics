//
// Created by neo on 2020/5/3.
//

#include "javascript/android/JSPageScopeTracer.h"
#include "V8Image.h"
#include "V8TemplateConfiguration.h"
#include "V8Platform.h"
#include "V8Constructor.h"
#include "V8TemplateCache.h"
#include "node/canvas/EventSender.h"

using namespace v8;

namespace AtomGraphics {

JSImageOnLoadEventListener::JSImageOnLoadEventListener(ImageBitmap *image) {
    EventSender::sender()->listenToEvent(EventTypeOnLoad, this, image);
}

JSImageOnLoadEventListener::~JSImageOnLoadEventListener() {
    EventSender::sender()->removeListener(EventTypeOnLoad, this);
}

void JSImageOnLoadEventListener::onEvent(EventType type, Element *image) {
    if (type == EventTypeOnLoad) {

        uint32_t newBytesCount = (dynamic_cast<ImageBitmap *>(image))->buffer()->bufferSize();
        JS::V8Platform::SharedPlatform()->getIsolate()->AdjustAmountOfExternalAllocatedMemory(
                newBytesCount - m_bytes);
        m_bytes = newBytesCount;

        if (m_onLoadFunction.IsEmpty()) {
            return;
        }

        GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([this]() -> void {
            JS::JSPageScopeTracer tracer(m_creatorPageScope);
            m_onLoadFunction.Get(m_isolate)->Call(
                    m_isolate->GetCurrentContext(),
                    m_isolate->GetCurrentContext()->Global(),
                    0,
                    nullptr);
        });
    }
}

JSOnPageDestroyEventListener::JSOnPageDestroyEventListener(V8Image *image,
                                                           JS::JSPageScope *creatorPageScope)
        : m_image(image),
          m_pageScope(creatorPageScope) {

    EventSender::sender()->listenToEvent(EventTypePageScopeWillBeDestroyed, this, nullptr);
}

void JSOnPageDestroyEventListener::onEvent(EventType type, Element *element) {

    if (JS::JSPageScopeTracer::CurrentPageScope() == m_pageScope) {
        m_pageScope = nullptr;
        m_image = nullptr;
        EventSender::sender()->removeListener(EventTypePageScopeWillBeDestroyed, this);
    }
}


V8Image::V8Image() : ScriptWrappable(), m_image(new ImageBitmap) {
    m_onLoadEventListener = std::make_unique<JSImageOnLoadEventListener>(m_image.get());
}

V8Image::~V8Image() {
    JS::V8Platform::SharedPlatform()
            ->getIsolate()->AdjustAmountOfExternalAllocatedMemory(
            -(m_image->buffer() ? m_image->buffer()->bufferSize() : 0));
}

const WrapperTypeInfo V8Image::wrapperTypeInfo = {
        domTemplate,
        "HTMLImageElement"
};

const WrapperTypeInfo *V8Image::getWrapperTypeInfo() const {
    return &wrapperTypeInfo;
}

void V8Image::Trace(V8GCMarkingVisitor *visitor) {
    visitor->Visit(this);
}

void V8Image::constructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
    if (!info.IsConstructCall()) {
        return;
    }

    if (ConstructorMode::Current(info.GetIsolate()) == ConstructorMode::kWrapExistingObject) {
        info.GetReturnValue().Set(info.Holder());
        return;
    }

    V8Image::constructor(info);
}

void V8Image::constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
    V8Image *scriptWrapper = new V8Image();
    v8::Local<v8::Object> wrapper = info.Holder();
    wrapper = scriptWrapper->AssociateWithWrapper(info.GetIsolate(),
                                                  &wrapperTypeInfo,
                                                  wrapper);
    info.GetReturnValue().Set(wrapper);
}

v8::Local<v8::FunctionTemplate>
V8Image::domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context) {
    return V8TemplateCache::ClassTemplate(isolate, context, &wrapperTypeInfo, InstallTemplate);
}

void V8Image::InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                              v8::Local<v8::FunctionTemplate> interfaceTemplate) {
    // Initialize the interface object's template.
    V8TemplateConfiguration::InitializeInterfaceTemplate(isolate, interfaceTemplate,
                                                         wrapperTypeInfo.interface_name,
                                                         kV8DefaultWrapperInternalFieldCount);
    interfaceTemplate->SetCallHandler(V8Image::constructorCallback);
    interfaceTemplate->SetLength(2);

    v8::Local<v8::ObjectTemplate> instanceTemplate = interfaceTemplate->InstanceTemplate();


    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(isolate, "src", NewStringType::kInternalized).ToLocalChecked(),
            GetSrc, SetSrc);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(
                    isolate, "width", NewStringType::kInternalized).ToLocalChecked(),
            GetWidth);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(
                    isolate, "height", NewStringType::kInternalized).ToLocalChecked(),
            GetHeight);

    instanceTemplate->SetAccessor(
            v8::String::NewFromUtf8(
                    isolate, "onload", NewStringType::kInternalized).ToLocalChecked(),
            GetOnLoad, SetOnLoad);

}

void V8Image::GetSrc(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info) {
    V8Image *v8Image = dynamic_cast<V8Image *>(ToScriptWrappable(info.Holder()));
    v8::Isolate *isolate = info.GetIsolate();
    info.GetReturnValue().Set(v8::String::NewFromUtf8(
            isolate, v8Image->m_image->getSrc().data(),
            NewStringType::kInternalized).ToLocalChecked());
}

void V8Image::SetSrc(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                     const v8::PropertyCallbackInfo<void> &info) {

    V8Image *v8Image = dynamic_cast<V8Image *>(ToScriptWrappable(info.Holder()));

    v8::String::Utf8Value utf8_value(info.GetIsolate(), value);
    if (v8Image->m_image->getSrc() != *utf8_value) {
        v8Image->m_image->setSrc(*utf8_value);
        v8Image->m_image->imageLoader()->updateFromElement();
    }

}

void V8Image::GetWidth(v8::Local<v8::String> name,
                       const v8::PropertyCallbackInfo<v8::Value> &info) {
    V8Image *v8Image = dynamic_cast<V8Image *>(ToScriptWrappable(info.Holder()));
    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), v8Image->m_image->width()));
}

void V8Image::GetHeight(v8::Local<v8::String> name,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    V8Image *v8Image = dynamic_cast<V8Image *>(ToScriptWrappable(info.Holder()));
    info.GetReturnValue().Set(v8::Integer::New(info.GetIsolate(), v8Image->m_image->height()));
}

void V8Image::GetOnLoad(v8::Local<v8::String> name,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
    V8Image *v8Image = dynamic_cast<V8Image *>(ToScriptWrappable(info.Holder()));
    Isolate *isolate = info.GetIsolate();
    if (v8Image->m_onLoadEventListener) {
        info.GetReturnValue().Set(v8Image->m_onLoadEventListener->m_onLoadFunction.Get(isolate));
    }
}

void V8Image::SetOnLoad(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info) {

    V8Image *v8Image = dynamic_cast<V8Image *>(ToScriptWrappable(info.Holder()));

    if (value->IsUndefined() || value->IsNull()) {
        v8Image->m_onLoadEventListener->m_onLoadFunction.Reset();
        return;
    }

    if (!value->IsFunction()) {
        return;
    }

    Local<Function> function = Local<Function>::Cast(value);
    JS::JSPageScope *pageScope = JS::JSPageScopeTracer::CurrentPageScope();
    v8::Isolate *isolate = info.GetIsolate();
    v8Image->m_onLoadEventListener->m_onLoadFunction = Global<Function>(isolate, function);
    v8Image->m_onLoadEventListener->m_creatorPageScope = pageScope;
    v8Image->m_onLoadEventListener->m_isolate = isolate;
}

}