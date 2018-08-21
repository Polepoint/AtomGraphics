//
// Created by neo on 2018/5/9.
//

#ifndef ATOMGRAPHICS_ATOMGRAPHICSJSLOADER_H
#define ATOMGRAPHICS_ATOMGRAPHICSJSLOADER_H

#include <JavaScriptCore/JSContextRef.h>
#include <string>
#include "JSObject.h"

namespace AtomGraphicsJSC {

    class JSObject;

    class JSContext {

    public:

        JSContext();

        JSValueRef evaluateScript(std::string script);

        void setProperty(std::string propertyName, JSObject *object);

        JSObject* getProperty(std::string propertyName);

        const JSContextRef getContextRef() const;

        virtual ~JSContext();

    private:

        JSContextGroupRef m_group;
        JSGlobalContextRef m_context;

    };
}


#endif //ATOMGRAPHICS_ATOMGRAPHICSJSLOADER_H
