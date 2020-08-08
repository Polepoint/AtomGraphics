//
// Created by neo on 2019/4/17.
//

#ifndef ATOMGRAPHICS_V8DATACONVERTOR_H
#define ATOMGRAPHICS_V8DATACONVERTOR_H

#include <cstdint>
#include <v8/v8.h>

namespace AtomGraphics {

enum IntegerConversionConfiguration {
    kNormalConversion,
    kEnforceRange,
    kClamp
};

uint32_t ToUInt32Slow(v8::Isolate *, v8::Local<v8::Value>, IntegerConversionConfiguration);

inline uint32_t ToUInt32(v8::Isolate *isolate, v8::Local<v8::Value> value,
                         IntegerConversionConfiguration configuration) {

    // Fast case. The value is already a 32-bit unsigned integer.
    if (value->IsUint32())
        return value.As<v8::Uint32>()->Value();

    // Fast case. The value is a 32-bit signed integer with NormalConversion
    // configuration.
    if (value->IsInt32() && configuration == kNormalConversion)
        return value.As<v8::Int32>()->Value();

    return ToUInt32Slow(isolate, value, configuration);
}

}

#endif //ATOMGRAPHICS_V8DATACONVERTOR_H
