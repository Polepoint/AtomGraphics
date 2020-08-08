//
// Created by neo on 2018/10/16.
//

#ifndef ATOMGRAPHICS_V8CANVAS_H
#define ATOMGRAPHICS_V8CANVAS_H

#include "V8JSContext.h"
#include "node/canvas/CanvasNode.h"
#include "V8CanvasRenderingContext.h"
#include "V8CSSStyleDeclaration.h"


namespace AtomGraphics {

class V8Canvas : public ScriptWrappable {
public:

    static V8Canvas *CreateCanvasWrappableFromCache(std::shared_ptr<Node>);

    ~V8Canvas();

    static const WrapperTypeInfo wrapperTypeInfo;

    const WrapperTypeInfo *getWrapperTypeInfo() const override;

    void Trace(V8GCMarkingVisitor *visitor) override;

    static CanvasNode *ToImpl(v8::Local<v8::Object> object) {
        if (CheckWrapperInfoMatch<V8Canvas>(object)) {
            return ToScriptWrappable(object)->ToImpl<V8Canvas>()->node();
        }

        return nullptr;
    }

private:

    std::shared_ptr<Node> m_node;
    V8CanvasRenderingContext *context_{nullptr};
    V8CSSStyleDeclaration *style_{nullptr};

    V8Canvas(const std::shared_ptr<Node> &node);

    static v8::Local<v8::FunctionTemplate>
    domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context);

    static void InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                v8::Local<v8::FunctionTemplate> interfaceTemplate);

    CanvasNode *node() const {
        return dynamic_cast<CanvasNode *>(m_node.get());
    }

    static void GetWidth(v8::Local<v8::String> name,
                         const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetWidth(v8::Local<v8::String> name, v8::Local<v8::Value>,
                         const v8::PropertyCallbackInfo<void> &info);

    static void GetHeight(v8::Local<v8::String> name,
                          const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetHeight(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info);


    static void GetClientWidth(v8::Local<v8::String> name,
                               const v8::PropertyCallbackInfo<v8::Value> &info);


    static void GetClientHeight(v8::Local<v8::String> name,
                                const v8::PropertyCallbackInfo<v8::Value> &info);

    static void
    GetStyle(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void GetContext(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void SetAttribute(const v8::FunctionCallbackInfo<v8::Value> &args);

};
}


#endif //ATOMGRAPHICS_V8CANVAS_H
