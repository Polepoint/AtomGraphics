//
// Created by neo on 2019-09-13.
//

#ifndef ATOMGRAPHICS_V8JSDOCUMENT_H
#define ATOMGRAPHICS_V8JSDOCUMENT_H

#include "JSExecutionChecker.h"

namespace AtomGraphics {

namespace JS {

class V8JSDocument {

public:

    static void SetupDocument(v8::Isolate *isolate, v8::Local<v8::Object> global);

private:

    static void CreateElement(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void GetElementById(const v8::FunctionCallbackInfo<v8::Value> &info);

};

}

}


#endif //ATOMGRAPHICS_V8JSDOCUMENT_H
