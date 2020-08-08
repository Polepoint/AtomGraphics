//
// Created by neo on 19-5-24.
//

#ifndef ATOMGRAPHICS_V8CONVERTER_H
#define ATOMGRAPHICS_V8CONVERTER_H


#include <v8/v8.h>
#include <string>

namespace AtomGraphics {

bool ConvertFromV8(v8::Isolate *isolate, v8::Local<v8::Value> input, std::string *result);

}


#endif //ATOMGRAPHICS_V8CONVERTER_H
