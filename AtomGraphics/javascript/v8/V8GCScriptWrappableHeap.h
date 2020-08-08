//
// Created by neo on 2019/5/6.
//

#ifndef ATOMGRAPHICS_V8GCSCRIPTWRAPPABLEHEAP_H
#define ATOMGRAPHICS_V8GCSCRIPTWRAPPABLEHEAP_H

#include <queue>
#include "V8GCMarkingVisitor.h"
#include "V8WrappableMarkingVisitor.h"
#include "V8Platform.h"

namespace AtomGraphics {


class HeapSlot {
public:
    HeapSlot(ScriptWrappable *w) : wrappable(w) {}

    ~HeapSlot() {}

    void finalize();

    ScriptWrappable *wrappable{nullptr};
    HeapSlot *next{nullptr};
    HeapSlot *pre{nullptr};
};


class V8GCScriptWrappableHeap {

public:
    static V8GCScriptWrappableHeap *Heap() {
        static V8GCScriptWrappableHeap *singleton;
        if (!singleton) {
            singleton = new V8GCScriptWrappableHeap(JS::V8Platform::SharedPlatform()->getIsolate());
        }

        return singleton;
    }

    void registerWrappable(ScriptWrappable *wrappable);

    void CollectGarbage();

    void CompleteSweep();

private:

    v8::Isolate *m_isolate;

    HeapSlot *m_heapHeader{nullptr};
    HeapSlot *m_heapTail{nullptr};

    std::vector<ScriptWrappable *> marking_worklist_;

private:

    V8GCScriptWrappableHeap(v8::Isolate *isolate) : m_isolate(isolate) {}

    void VisitWrappers();

    void TraceAndMarkWrappables();

    friend V8WrappableMarkingVisitor;
};

}


#endif //ATOMGRAPHICS_V8GCSCRIPTWRAPPABLEHEAP_H
