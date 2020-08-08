//
// Created by neo on 2019/4/16.
//

#include "V8TemplateCache.h"

namespace AtomGraphics {

v8::Local<v8::FunctionTemplate> V8TemplateCache::ClassTemplate(v8::Isolate *isolate,
                                                               v8::Local<v8::Context> context,
                                                               const WrapperTypeInfo *wrapper_type_info,
                                                               InstallTemplateFunction configure_dom_class_template) {

    v8::Local<v8::FunctionTemplate> interface_template = FindInterfaceTemplate(isolate, wrapper_type_info);
    if (!interface_template.IsEmpty()) {
        return interface_template;
    }

    interface_template = v8::FunctionTemplate::New(isolate);
    configure_dom_class_template(isolate, context, interface_template);
    CacheInterfaceTemplate(isolate, wrapper_type_info, interface_template);
    return interface_template;

}


V8TemplateCache *V8TemplateCache::SharedCache() {
    static V8TemplateCache *sharedCache = nullptr;
    if (!sharedCache) {
        sharedCache = new V8TemplateCache();
    }

    return sharedCache;
}

v8::Local<v8::FunctionTemplate> V8TemplateCache::FindInterfaceTemplate(
        v8::Isolate *isolate, const WrapperTypeInfo *type) {
    V8TemplateCache *sharedCache = SharedCache();
    auto result = sharedCache->m_templateMap.find(type);
    if (result != sharedCache->m_templateMap.end())
        return result->second.Get(isolate);
    return v8::Local<v8::FunctionTemplate>();
}

void V8TemplateCache::CacheInterfaceTemplate(v8::Isolate *isolate, const WrapperTypeInfo *typeInfo,
                                             v8::Local<v8::FunctionTemplate> value) {
    SharedCache()->m_templateMap.insert(std::make_pair(typeInfo, v8::Eternal<v8::FunctionTemplate>(isolate, value)));
}

}