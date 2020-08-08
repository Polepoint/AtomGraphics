//
// Created by neo on 2019/2/20.
//

#ifndef ATOMGRAPHICS_V8CANVASSTYLE_H
#define ATOMGRAPHICS_V8CANVASSTYLE_H

#include <v8/v8.h>
#include "node/canvas/CanvasStyle.h"
#include "ScriptWrappable.h"

namespace AtomGraphics {

class V8CanvasStyle : public ScriptWrappable {

public:

    V8CanvasStyle(std::unique_ptr<CanvasStyle> style);

    static const WrapperTypeInfo wrapperTypeInfo;

    const WrapperTypeInfo *getWrapperTypeInfo() const override;

    void Trace(V8GCMarkingVisitor *visitor) override;

    static CanvasStyle *ToImpl(v8::Local<v8::Object> object) {
        return ToScriptWrappable(object)->ToImpl<V8CanvasStyle>()->m_style.get();
    }

    static bool hasInstance(v8::Local<v8::Value> value, v8::Isolate *isolate) {
        return HasInstance(value, isolate, &wrapperTypeInfo);
    }

private:
    std::unique_ptr<CanvasStyle> m_style;

    static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context);

    static void InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                v8::Local<v8::FunctionTemplate> interfaceTemplate);

    static void AddColorStop(const v8::FunctionCallbackInfo<v8::Value> &args);
};

}


#endif //ATOMGRAPHICS_V8CANVASSTYLE_H
