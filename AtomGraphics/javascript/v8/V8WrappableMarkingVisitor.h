//
// Created by neo on 2019/5/7.
//

#ifndef ATOMGRAPHICS_V8GCSCRIPTWRAPPERMARKINGVISITOR_H
#define ATOMGRAPHICS_V8GCSCRIPTWRAPPERMARKINGVISITOR_H

#include "V8GCMarkingVisitor.h"

namespace AtomGraphics {

class V8GCScriptWrappableHeap;

class ScriptWrappable;

class V8WrappableMarkingVisitor : public V8GCMarkingVisitor {

public:

    V8WrappableMarkingVisitor(V8GCScriptWrappableHeap *heap);

    void Visit(ScriptWrappable *wrappable) override;

private:
    V8GCScriptWrappableHeap *m_heap;

    void MarkWrapper(ScriptWrappable *wrappable);
};

}


#endif //ATOMGRAPHICS_V8GCSCRIPTWRAPPERMARKINGVISITOR_H
