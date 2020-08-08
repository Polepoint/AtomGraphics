//
// Created by neo on 2019/4/15.
//

#include "base/Clock.h"
#include "V8ReferenceMarkingVisitor.h"
#include "WrapperTypeInfo.h"

namespace AtomGraphics {

using namespace v8;


void V8ReferenceMarkingVisitor::RegisterV8References(
        const std::vector<std::pair<void *, void *> > &embedder_fields) {
    for (auto &pair : embedder_fields) {
        registerV8Reference(pair);
    }
}

void V8ReferenceMarkingVisitor::registerV8Reference(const std::pair<void *, void *> &internal_fields) {

    WrapperTypeInfo *wrapper_type_info = reinterpret_cast<WrapperTypeInfo *>(internal_fields.first);
    visitWithWrappers(static_cast<ScriptWrappable *>(internal_fields.second));
}

void V8ReferenceMarkingVisitor::visitWithWrappers(ScriptWrappable *wrappable) {
    if (wrappable->isV8WrapperMarked())
        return;

    wrappable->markV8Wrapper();
    marking_deque_.push(wrappable);
}

void V8ReferenceMarkingVisitor::PerformCleanup() {
    marking_deque_ = std::queue<ScriptWrappable *>();
}

void V8ReferenceMarkingVisitor::TracePrologue() {
    PerformCleanup();
}

void V8ReferenceMarkingVisitor::TraceEpilogue() {

}

bool V8ReferenceMarkingVisitor::AdvanceTracing(double deadline_in_ms, AdvanceTracingActions actions) {
    //Notify all the marked alive wrappers to v8

    TimeInterval deadline = Clock::now() + deadline_in_ms / 1000;

    while (Clock::now() < deadline) {
        if (marking_deque_.empty()) {
            return true;
        }
        marking_deque_.front()->Trace(this);
        marking_deque_.pop();
    }

    return false;
}

void V8ReferenceMarkingVisitor::Visit(ScriptWrappable *scriptWrappable) {
    scriptWrappable->wrapper().RegisterExternalReference(isolate());
}

void V8ReferenceMarkingVisitor::EnterFinalPause() {

}

void V8ReferenceMarkingVisitor::AbortTracing() {
    PerformCleanup();
}

}