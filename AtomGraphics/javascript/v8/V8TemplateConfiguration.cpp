//
// Created by neo on 2019/4/17.
//

#include "V8TemplateConfiguration.h"
#include "WrapperTypeInfo.h"

namespace AtomGraphics {

inline v8::Local<v8::String> V8AtomicString(v8::Isolate *isolate,
                                            const char *string) {
    if (!string || string[0] == '\0')
        return v8::String::Empty(isolate);
    return v8::String::NewFromOneByte(
            isolate, reinterpret_cast<const uint8_t *>(string),
            v8::NewStringType::kInternalized, static_cast<int>(strlen(string)))
            .ToLocalChecked();
}

void V8TemplateConfiguration::InitializeInterfaceTemplate(v8::Isolate *isolate,
                                                          v8::Local<v8::FunctionTemplate> interface_template,
                                                          const char *interface_name,
                                                          uint32_t v8_internal_field_count) {
    interface_template->SetClassName(V8AtomicString(isolate, interface_name));
    interface_template->ReadOnlyPrototype();
    v8::Local<v8::ObjectTemplate> instance_template = interface_template->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> prototype_template = interface_template->PrototypeTemplate();
    instance_template->SetInternalFieldCount(v8_internal_field_count);
    SetClassString(isolate, prototype_template, interface_name);
}

void V8TemplateConfiguration::SetClassString(
        v8::Isolate *isolate,
        v8::Local<v8::ObjectTemplate> object_template,
        const char *class_string) {
    object_template->Set(
            v8::Symbol::GetToStringTag(isolate),
            V8AtomicString(isolate, class_string),
            static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontEnum));
}

}