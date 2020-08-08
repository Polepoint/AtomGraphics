//
// Created by neo on 2019/3/31.
//

#ifndef ATOMGRAPHICS_JSPAGESCOPETRACER_H
#define ATOMGRAPHICS_JSPAGESCOPETRACER_H

#include <stack>
#include "JSPageScope.h"

namespace AtomGraphics {

namespace JS {

class JSPageScopeTracer final {

public:

    static JSPageScope *CurrentPageScope();

    JSPageScopeTracer(JSPageScope *pageScope);

    ~JSPageScopeTracer();

private:

    JSPageScope *m_pageScope;

};

}

}


#endif //ATOMGRAPHICS_JSPAGESCOPETRACER_H
