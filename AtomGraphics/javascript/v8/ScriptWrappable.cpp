//
// Created by neo on 2019/4/16.
//

#include "ScriptWrappable.h"
#include "V8TemplateConfiguration.h"
#include "WrapperCache.h"
#include "WrapperTypeInfo.h"
#include "V8Constructor.h"
#include "V8GCScriptWrappableHeap.h"
#include "V8TemplateCache.h"

namespace AtomGraphics {

WrapperCache *ScriptWrappable::SharedWrapperCache() {
    static WrapperCache *wrapperCache = nullptr;
    if (!wrapperCache) {
        wrapperCache = new WrapperCache();
    }

    return wrapperCache;
}

ScriptWrappable::ScriptWrappable() {
    V8GCScriptWrappableHeap::Heap()->registerWrappable(this);
}

v8::Local<v8::Object> ScriptWrappable::Wrap(v8::Isolate *isolate, v8::Local<v8::Context> context) {
    const WrapperTypeInfo *type = getWrapperTypeInfo();

    v8::Local<v8::Object> wrapper = CreateWrapperFromCache(isolate, context, type);

    return AssociateWithWrapper(isolate, type, wrapper);
}

v8::Local<v8::Object> ScriptWrappable::CreateWrapperFromCache(v8::Isolate *isolate,
                                                              v8::Local<v8::Context> context,
                                                              const WrapperTypeInfo *wrapperTypeInfo) {
    v8::Local<v8::Object> wrapper = SharedWrapperCache()->get(isolate, wrapperTypeInfo);
    if (wrapper.IsEmpty()) {
        v8::Context::Scope scope(context);
        v8::Local<v8::Function> interface_object = ConstructorForType(isolate, context, wrapperTypeInfo);

        ConstructorMode constructor_mode(isolate);
        wrapper = interface_object->NewInstanceWithSideEffectType(
                isolate->GetCurrentContext(), 0, nullptr, v8::SideEffectType::kHasNoSideEffect).ToLocalChecked();

        SharedWrapperCache()->set(isolate, wrapperTypeInfo, wrapper);
    }

    return wrapper->Clone();
}


v8::Local<v8::Function> ScriptWrappable::ConstructorForType(v8::Isolate *isolate,
                                                            v8::Local<v8::Context> context,
                                                            const WrapperTypeInfo *type) {
    v8::Local<v8::FunctionTemplate> interface_template = type->domTemplate(isolate, context);

    v8::Local<v8::Function> interface_object;
    if (interface_template->GetFunction(context).ToLocal(&interface_object)) {
        return interface_object;
    } else {
        //log error
        return v8::Local<v8::Function>();
    }
}


v8::Local<v8::Object> ScriptWrappable::AssociateWithWrapper(
        v8::Isolate *isolate,
        const WrapperTypeInfo *wrapper_type_info,
        v8::Local<v8::Object> wrapper) {
    if (SetWrapper(isolate, wrapper, nullptr)) {
        SetNativeInfo(wrapper, wrapper_type_info);
    }

    return wrapper;
}

bool ScriptWrappable::SetWrapper(v8::Isolate *isolate, v8::Local<v8::Object> &wrapper,
                                 const WrapperTypeInfo *wrapper_type_info) {

    if (!wrapper_.IsEmpty()) {
        wrapper = v8::Local<v8::Object>::New(isolate, wrapper_);
        return false;
    }

    wrapper_.Reset(isolate, wrapper);
    wrapper_.SetWeak();
    wrapper_type_info->ConfigureWrapper(&wrapper_);
    return true;
}

void ScriptWrappable::SetNativeInfo(v8::Local<v8::Object> &wrapper, const WrapperTypeInfo *wrapper_type_info) {
    int indices[] = {kV8DOMWrapperObjectIndex, kV8DOMWrapperTypeIndex};
    void *values[] = {this, const_cast<WrapperTypeInfo *>(wrapper_type_info)};
    wrapper->SetAlignedPointerInInternalFields(2, indices, values);
}

bool ScriptWrappable::HasInstance(v8::Local<v8::Value> value, v8::Isolate *isolate,
                                  const WrapperTypeInfo *wrapperTypeInfo) {
    v8::Local<v8::FunctionTemplate> templ = V8TemplateCache::FindInterfaceTemplate(isolate, wrapperTypeInfo);
    if (templ.IsEmpty()) {
        return false;
    }

    return templ->HasInstance(value);
}

}