//
// Created by neo on 2019/4/19.
//

#ifndef ATOMGRAPHICS_V8GCCONTROLLER_H
#define ATOMGRAPHICS_V8GCCONTROLLER_H

#include <v8/v8.h>
#include "ScriptWrappable.h"
#include "V8GCMarkingVisitor.h"

namespace AtomGraphics {

class V8GCController {

public:

    static void GcPrologue(v8::Isolate *isolate, v8::GCType type, v8::GCCallbackFlags flags);

    static void GcEpilogue(v8::Isolate *isolate, v8::GCType type, v8::GCCallbackFlags flags);

    static void TraceWrappers(v8::Isolate *isolate, AtomGraphics::V8GCMarkingVisitor *visitor);
};

}


#endif //ATOMGRAPHICS_V8GCCONTROLLER_H
