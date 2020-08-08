//
// Created by neo on 2019-09-18.
//

#ifndef ATOMGRAPHICS_JSPAGESCOPE_H
#define ATOMGRAPHICS_JSPAGESCOPE_H

#import "JSPageScope.h"

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

#endif //ATOMGRAPHICS_JSPAGESCOPE_H
