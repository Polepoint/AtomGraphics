//
// Created by neo on 2019/4/17.
//

#include "V8IsolateData.h"

namespace AtomGraphics {

void V8IsolateData::Initialize(v8::Isolate *isolate) {
    V8IsolateData *data = new V8IsolateData();
    isolate->SetData(IsolateDataType::kIsolateDataDefault, data);
    isolate->SetEmbedderHeapTracer(data->GetScriptWrappableMarkingVisitor());
}

V8IsolateData::V8IsolateData()
        : constructor_mode_(ConstructorMode::Mode::kCreateNewObject),
          script_wrappable_visitor_(new V8ReferenceMarkingVisitor) {

}


}