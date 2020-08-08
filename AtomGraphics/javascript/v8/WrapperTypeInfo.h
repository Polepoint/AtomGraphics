//
// Created by neo on 2019/4/16.
//

#ifndef ATOMGRAPHICS_WRAPPERTYPEINFO_H
#define ATOMGRAPHICS_WRAPPERTYPEINFO_H

#include <v8/v8.h>

namespace AtomGraphics {

class ScriptWrappable;

enum InternalFields {
    kWrapperInfoIndex,
    kEncodedValueIndex,
    kNumberOfInternalFields,
};

static const int kV8DOMWrapperTypeIndex = static_cast<int>(kWrapperInfoIndex);
static const int kV8DOMWrapperObjectIndex = static_cast<int>(kEncodedValueIndex);

static const int kV8PrototypeInternalFieldcount = 1;

static const int kV8DefaultWrapperInternalFieldCount = static_cast<uint32_t>(InternalFields::kNumberOfInternalFields);


static const int kV8DefaultWrapperObjectClassId = 1;

typedef v8::Local<v8::FunctionTemplate> (*DomTemplateFunction)(v8::Isolate *, v8::Local<v8::Context>);

struct WrapperTypeInfo {

    static const WrapperTypeInfo *Unwrap(v8::Local<v8::Value> type_info_wrapper) {
        return reinterpret_cast<const WrapperTypeInfo *>(
                v8::External::Cast(*type_info_wrapper)->Value());
    }

    v8::Local<v8::FunctionTemplate> domTemplate(
            v8::Isolate *isolate,
            v8::Local<v8::Context> world) const {
        return dom_template_function(isolate, world);
    }


    void ConfigureWrapper(v8::Persistent<v8::Object> *wrapper) const {
        wrapper->SetWrapperClassId(kV8DefaultWrapperObjectClassId);
    }

    DomTemplateFunction dom_template_function;
    const char *const interface_name;

};

template<typename T, int offset>
inline T *GetInternalField(const v8::PersistentBase<v8::Object> &persistent) {
    return reinterpret_cast<T *>(v8::Object::GetAlignedPointerFromInternalField(persistent, offset));
}

template<typename T, int offset>
inline T *GetInternalField(v8::Local<v8::Object> wrapper) {
    return reinterpret_cast<T *>(
            wrapper->GetAlignedPointerFromInternalField(offset));
}

inline const WrapperTypeInfo *ToWrapperTypeInfo(const v8::PersistentBase<v8::Object> &wrapper) {
    return GetInternalField<WrapperTypeInfo, kV8DOMWrapperTypeIndex>(wrapper);
}

inline const WrapperTypeInfo *ToWrapperTypeInfo(v8::Local<v8::Object> wrapper) {
    return GetInternalField<WrapperTypeInfo, kV8DOMWrapperTypeIndex>(wrapper);
}

inline ScriptWrappable *ToScriptWrappable(const v8::PersistentBase<v8::Object> &wrapper) {
    return GetInternalField<ScriptWrappable, kV8DOMWrapperObjectIndex>(wrapper);
}

inline ScriptWrappable *ToScriptWrappable(v8::Local<v8::Object> wrapper) {
    return GetInternalField<ScriptWrappable, kV8DOMWrapperObjectIndex>(wrapper);
}

}


#endif //ATOMGRAPHICS_WRAPPERTYPEINFO_H
