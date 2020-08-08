//
// Created by neo on 2019/4/17.
//

#ifndef ATOMGRAPHICS_V8ISOLATEDATA_H
#define ATOMGRAPHICS_V8ISOLATEDATA_H


#include <v8/v8.h>
#include "V8ReferenceMarkingVisitor.h"
#include "V8Constructor.h"

namespace AtomGraphics {


enum IsolateDataType {
    kIsolateDataDefault
};

class V8IsolateData {

public:

    static void Initialize(v8::Isolate *isolate);

    static V8IsolateData *From(v8::Isolate *isolate) {
        return static_cast<V8IsolateData *>(
                isolate->GetData(IsolateDataType::kIsolateDataDefault));
    }

    V8ReferenceMarkingVisitor *GetScriptWrappableMarkingVisitor() const {
        return script_wrappable_visitor_.get();
    }

    ConstructorMode::Mode constructor_mode_;

private:

    std::unique_ptr<V8ReferenceMarkingVisitor> script_wrappable_visitor_;

    V8IsolateData();
};

}


#endif //ATOMGRAPHICS_V8ISOLATEDATA_H
