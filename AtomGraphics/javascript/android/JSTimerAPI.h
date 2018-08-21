//
// Created by neo on 2018/7/29.
//

#ifndef ATOMGRAPHICS_JSTIMERAPI_H
#define ATOMGRAPHICS_JSTIMERAPI_H

#include <jsbridge/JSObject.h>

namespace AtomGraphicsJSC {

    class JSTimerAPI {
    public:
        static void
        InitJSTimerAPIs(JSContext *context, JSObject *windowRef, AtomGraphics::GraphicsPageContext *pageContext);

    };
}


#endif //ATOMGRAPHICS_JSTIMERAPI_H
