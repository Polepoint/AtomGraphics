//
// Created by neo on 2018/5/18.
//

#ifndef ATOMGRAPHICS_JAVASCRIPTINTERFACE_H
#define ATOMGRAPHICS_JAVASCRIPTINTERFACE_H

#include <jni.h>
#include "node/canvas/AtomCanvasContext2D.h"
#include "jsbridge/JSContext.h"
#include "node/AtomNode.h"

namespace AtomGraphicsJSC {

    class JavaScriptInterface {

    public:

        JavaScriptInterface(AtomGraphics::GraphicsPageContext *pageContext, jobject interfaceRef);

        void evaluateScript(const char *script);

    private:

        AtomGraphics::GraphicsPageContext *m_pageContext;

        JSContext *m_context{nullptr};

        jobject m_interfaceRef{nullptr};

        void initContext();

        void initAPIs();

        void preloadFiles();
    };
}


#endif //ATOMGRAPHICS_JAVASCRIPTINTERFACE_H
