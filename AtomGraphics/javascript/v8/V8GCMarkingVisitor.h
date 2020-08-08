//
// Created by neo on 2019/5/3.
//

#ifndef ATOMGRAPHICS_V8GCMARKINGVISITOR_H
#define ATOMGRAPHICS_V8GCMARKINGVISITOR_H

namespace AtomGraphics {

class ScriptWrappable;

class V8GCMarkingVisitor {

public:

    virtual void Visit(ScriptWrappable *wrappable) = 0;

};

}


#endif //ATOMGRAPHICS_V8GCMARKINGVISITOR_H
