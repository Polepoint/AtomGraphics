//
// Created by neo on 2019/4/19.
//

#include <v8/v8.h>
#include "V8GCController.h"
#include "V8GCMarkingVisitor.h"
#include "V8GCScriptWrappableHeap.h"

namespace AtomGraphics {

class MinorGCUnmodifiedWrapperVisitor : public v8::PersistentHandleVisitor {
public:
    explicit MinorGCUnmodifiedWrapperVisitor(v8::Isolate *isolate)
            : isolate_(isolate) {}

    void VisitPersistentHandle(v8::Persistent<v8::Value> *value,
                               uint16_t class_id) override {
        if (class_id != kV8DefaultWrapperObjectClassId)
            return;

        v8::Persistent<v8::Object>::Cast(*value).MarkActive();
    }

private:
    v8::Isolate *isolate_;
};


void VisitWeakHandlesForMinorGC(v8::Isolate *isolate) {
    MinorGCUnmodifiedWrapperVisitor visitor(isolate);
    isolate->VisitWeakHandles(&visitor);
}


void V8GCController::GcPrologue(v8::Isolate *isolate,
                                v8::GCType type,
                                v8::GCCallbackFlags flags) {
//    ScriptForbiddenScope::Enter();
//
//    // Attribute garbage collection to the all frames instead of a specific
//    // frame.
//    if (BlameContext* blame_context =
//                Platform::Current()->GetTopLevelBlameContext())
//        blame_context->Enter();

    v8::HandleScope scope(isolate);
    switch (type) {
        case v8::GCType::kGCTypeScavenge: {
            VisitWeakHandlesForMinorGC(isolate);
            break;
        }
        case v8::GCType::kGCTypeMarkSweepCompact: {
//            if (ThreadState::Current())
//                ThreadState::Current()->WillStartV8GC(BlinkGC::kV8MajorGC);
            break;
        }
        case v8::GCType::kGCTypeIncrementalMarking: {
//            ThreadState::Current()->WillStartV8GC(BlinkGC::kV8MajorGC);
            break;
        }
        case v8::GCType::kGCTypeProcessWeakCallbacks: {
            break;
        }

        default:
            break;
    }

}

void V8GCController::GcEpilogue(v8::Isolate *isolate,
                                v8::GCType type,
                                v8::GCCallbackFlags flags) {
    // v8::kGCCallbackFlagCollectAllAvailableGarbage is used when V8 handles
    // low memory notifications.
    if ((flags & v8::kGCCallbackFlagForced) ||
            (flags & v8::kGCCallbackFlagCollectAllAvailableGarbage) ||
            (flags & v8::kGCCallbackFlagCollectAllExternalMemory)) {
        V8GCScriptWrappableHeap::Heap()->CollectGarbage();
    }
}

namespace {

class ScriptWrapperTracer final : public v8::PersistentHandleVisitor {
public:
    explicit ScriptWrapperTracer(V8GCMarkingVisitor *visitor)
            : visitor_(visitor) {
    }

    void VisitPersistentHandle(v8::Persistent<v8::Value> *value,
                               uint16_t class_id) final {

        WrapperTypeInfo *wrapper_type_info = const_cast<WrapperTypeInfo *>(
                ToWrapperTypeInfo(v8::Persistent<v8::Object>::Cast(*value)));

        // WrapperTypeInfo pointer may have been cleared before termination GCs on
        // worker threads.
        if (!wrapper_type_info)
            return;

        visitor_->Visit(ToScriptWrappable(v8::Persistent<v8::Object>::Cast(*value)));
    }

private:
    V8GCMarkingVisitor *const visitor_;
};

}


void V8GCController::TraceWrappers(v8::Isolate *isolate, V8GCMarkingVisitor *visitor) {
    ScriptWrapperTracer tracer(visitor);
    isolate->VisitHandlesWithClassIds(&tracer);
}


};