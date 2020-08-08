//
// Created by neo on 19-5-24.
//

#include "V8Converter.h"


namespace AtomGraphics {

bool ConvertFromV8(v8::Isolate *isolate, v8::Local <v8::Value> input, std::string *result) {

    if (!input->IsString())
        return false;
    v8::Local <v8::String> str = v8::Local<v8::String>::Cast(input);
    int length = str->Utf8Length(isolate);
    result->resize(length);
    str->WriteUtf8(isolate, &(*result)[0], length, nullptr, v8::String::NO_NULL_TERMINATION);
    return true;
}

}