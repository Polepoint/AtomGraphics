//
// Created by neo on 2019/4/17.
//

#include "V8Constructor.h"
#include "V8IsolateData.h"

namespace AtomGraphics {

ConstructorMode::ConstructorMode(v8::Isolate *isolate) : isolate_(isolate) {
    V8IsolateData *data = V8IsolateData::From(isolate_);
    previous_ = data->constructor_mode_;
    data->constructor_mode_ = kWrapExistingObject;
}

ConstructorMode::~ConstructorMode() {
    V8IsolateData *data = V8IsolateData::From(isolate_);
    data->constructor_mode_ = previous_;
}

bool ConstructorMode::Current(v8::Isolate *isolate) {
    return V8IsolateData::From(isolate)->constructor_mode_;
}

}