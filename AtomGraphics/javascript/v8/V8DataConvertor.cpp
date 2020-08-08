//
// Created by neo on 2019/4/17.
//

#include <cmath>
#include "V8DataConvertor.h"


namespace AtomGraphics {

const uint32_t kMaxUInt32 = 0xffffffff;

static double EnforceRange(double x, double minimum, double maximum) {
    if (std::isnan(x) || std::isinf(x)) {
        return 0;
    }
    x = trunc(x);
    if (x < minimum || x > maximum) {
        return 0;
    }
    return x;
}

uint32_t ToUInt32Slow(v8::Isolate *isolate, v8::Local<v8::Value> value, IntegerConversionConfiguration configuration) {

    if (value->IsInt32()) {
        int32_t result = value.As<v8::Int32>()->Value();
        if (result >= 0)
            return static_cast<uint32_t>(result);
        if (configuration == kEnforceRange) {
            return 0;
        }
        return static_cast<uint32_t>(result);
    }

    // Can the value be converted to a number?
    v8::TryCatch block(isolate);
    v8::Local<v8::Number> number_object;
    if (!value->ToNumber(isolate->GetCurrentContext()).ToLocal(&number_object)) {
        return 0;
    }

    if (configuration == kEnforceRange) {
        return EnforceRange(number_object->Value(), 0, kMaxUInt32);
    }

    double number_value = number_object->Value();

    if (std::isnan(number_value))
        return 0;

    if (configuration == kClamp)
        return static_cast<uint32_t>(number_value);

    if (std::isinf(number_value))
        return 0;

    uint32_t result;
    if (!number_object->Uint32Value(isolate->GetCurrentContext()).To(&result)) {
        return 0;
    }
    return result;
}

}