//
// Created by neo on 2019/4/15.
//

#ifndef ATOMGRAPHICS_V8GCWRAPPABLEMARKINGVISITOR_H
#define ATOMGRAPHICS_V8GCWRAPPABLEMARKINGVISITOR_H

#include <v8/v8.h>
#include <mutex>
#include <queue>
#include "ScriptWrappable.h"
#include "V8GCMarkingVisitor.h"

namespace AtomGraphics {

class V8ReferenceMarkingVisitor :
        public V8GCMarkingVisitor,
        public v8::EmbedderHeapTracer {

public:

    void RegisterV8References(const std::vector<std::pair<void *, void *> > &embedder_fields) override;

    void TracePrologue() override;

    void TraceEpilogue() override;

    bool AdvanceTracing(double deadline_in_ms, AdvanceTracingActions actions) override;

    void EnterFinalPause() override;

    void AbortTracing() override;

    void Visit(ScriptWrappable *scriptWrappable) override;

private:

    std::queue<ScriptWrappable *> marking_deque_;

    void registerV8Reference(const std::pair<void *, void *> &internal_fields);

    void visitWithWrappers(ScriptWrappable *wrappable);

    void PerformCleanup();
};

}


#endif //ATOMGRAPHICS_V8GCWRAPPABLEMARKINGVISITOR_H
