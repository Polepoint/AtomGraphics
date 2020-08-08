//
// Created by neo on 2019/4/18.
//

#include "WrapperCache.h"

namespace AtomGraphics {

v8::Local<v8::Object> WrapperCache::get(v8::Isolate *isolate, const WrapperTypeInfo *wrapperTypeInfo) {
    auto it = wrapper_map_cache_.find(wrapperTypeInfo);
    if (it == wrapper_map_cache_.end()) {
        return v8::Local<v8::Object>();
    }

    return it->second->value_.Get(isolate);
}

void WrapperCache::set(v8::Isolate *isolate,
                       const WrapperTypeInfo *wrapperTypeInfo,
                       v8::Local<v8::Object> instance_template) {
    auto it = wrapper_map_cache_.find(wrapperTypeInfo);
    if (it != wrapper_map_cache_.end()) {
        return;
    }

    v8::Global<v8::Object> persistent(isolate, instance_template);
    auto mapValue = new GlobalValueReference();
    mapValue->value_.Reset(isolate, instance_template);
    wrapper_map_cache_.insert(std::pair<const WrapperTypeInfo *, GlobalValueReference *>(wrapperTypeInfo, mapValue));
}

}