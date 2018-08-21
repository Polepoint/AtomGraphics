//
// Created by neo on 2018/7/29.
//

#ifndef ATOMGRAPHICS_CANVASJAVASCRIPTAPI_H
#define ATOMGRAPHICS_CANVASJAVASCRIPTAPI_H

#include "node/canvas/AtomCanvasNode.h"
#include "jsbridge/JSObject.h"

namespace AtomGraphicsJSC {

    class CanvasJavaScriptAPI {
    public:
        static JSObject *JSObjectForCanvasNode(JSContext *context, AtomGraphics::CanvasNode *node);

    private:

        static void InitCanvasAPIs(JSContext *, JSObject *);
    };
}


#endif //ATOMGRAPHICS_CANVASJAVASCRIPTAPI_H
