//
// Created by neo on 2019/5/6.
//

#include "base/AtomLog.h"
#include "V8GCScriptWrappableHeap.h"
#include "V8GCMarkingVisitor.h"
#include "V8GCController.h"
#include "V8ReferenceMarkingVisitor.h"


namespace AtomGraphics {

void HeapSlot::finalize() {
    if (wrappable) {
        delete wrappable;
    }
}

void V8GCScriptWrappableHeap::registerWrappable(ScriptWrappable *wrappable) {
    if (!m_heapHeader) {
        m_heapHeader = new HeapSlot(nullptr);
        m_heapTail = m_heapHeader;
    }
    HeapSlot *slot = new HeapSlot(wrappable);
    slot->wrappable = wrappable;
    m_heapTail->next = slot;
    slot->pre = m_heapTail;

    m_heapTail = slot;
}

void V8GCScriptWrappableHeap::CollectGarbage() {
    marking_worklist_.clear();
    VisitWrappers();
    TraceAndMarkWrappables();
    CompleteSweep();
}

void V8GCScriptWrappableHeap::VisitWrappers() {
    V8WrappableMarkingVisitor visitor(this);
    V8GCController::TraceWrappers(m_isolate, &visitor);
}

void V8GCScriptWrappableHeap::TraceAndMarkWrappables() {
    V8WrappableMarkingVisitor visitor(this);
    for (ScriptWrappable *scriptWrappable : marking_worklist_) {
        scriptWrappable->Trace(&visitor);
    }
};

void V8GCScriptWrappableHeap::CompleteSweep() {
    HeapSlot *slot = m_heapHeader->next;
    while (slot) {
        ScriptWrappable *wrappable = slot->wrappable;
        if (!wrappable->isWrappableMarked()) {
            HeapSlot *next = slot->next;
            HeapSlot *pre = slot->pre;
            pre->next = next;
            if (next) {
                next->pre = pre;
            }
            slot->finalize();
            delete slot;
            slot = next;
        } else {
            wrappable->unmarkWrappble();
            slot = slot->next;
        }
    }
}

}