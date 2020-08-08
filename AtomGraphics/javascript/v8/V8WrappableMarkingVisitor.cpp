//
// Created by neo on 2019/5/7.
//

#include "ScriptWrappable.h"
#include "V8WrappableMarkingVisitor.h"
#include "V8GCScriptWrappableHeap.h"

namespace AtomGraphics {

V8WrappableMarkingVisitor::V8WrappableMarkingVisitor(V8GCScriptWrappableHeap *heap) : m_heap(heap) {}

void V8WrappableMarkingVisitor::Visit(ScriptWrappable *wrappable) {
    MarkWrapper(wrappable);
}

void V8WrappableMarkingVisitor::MarkWrapper(ScriptWrappable *wrappable) {
    if(!wrappable->isWrappableMarked()){
        wrappable->markWrappable();
        m_heap->marking_worklist_.push_back(wrappable);
    }
}

}