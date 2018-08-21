//
// Created by neo on 2018/7/29.
//

#ifndef ATOMGRAPHICS_CANVASCONTEXTJAVASCRIPTAPI_H
#define ATOMGRAPHICS_CANVASCONTEXTJAVASCRIPTAPI_H

#include <jsbridge/JSObject.h>
#include <node/canvas/AtomCanvasContext2D.h>

namespace AtomGraphicsJSC {

    class CanvasContextJavaScriptAPI {
    public:

        static JSObject *JSObjectForCanvasContext(JSContext *, AtomGraphics::CanvasContext2D *);

    private:

        static void InitContextAPIs(AtomGraphicsJSC::JSObject *canvasContext2dJS);

        static JSObject *CreateLinearGradient(JSContext *ctx, AtomGraphics::CanvasContext2D *context2D,
                                              float x0, float y0, float x1, float y1);

        static JSObject *CreateRadialGradient(JSContext *ctx, AtomGraphics::CanvasContext2D *context2D,
                                              float x0, float y0, float r0, float x1, float y1, float r1);


        static void InitLinearGradientAPIs(JSObject *gradient);

        static void InitRadialGradientAPIs(JSObject *gradient);
    };

}


#endif //ATOMGRAPHICS_CANVASCONTEXTJAVASCRIPTAPI_H
