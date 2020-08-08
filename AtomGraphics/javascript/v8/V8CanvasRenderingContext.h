//
// Created by neo on 2018/10/16.
//

#ifndef ATOMGRAPHICS_V8CANVASCONTEXT_H
#define ATOMGRAPHICS_V8CANVASCONTEXT_H

#include "node/canvas/CanvasRenderingContext2D.h"
#include "ScriptWrappable.h"
#include <v8/v8.h>

namespace AtomGraphics {

class V8CanvasRenderingContext : public ScriptWrappable {
public:

    V8CanvasRenderingContext(std::shared_ptr<CanvasRenderingContext2D> context);

    ~V8CanvasRenderingContext();

    static const WrapperTypeInfo wrapperTypeInfo;

    const WrapperTypeInfo *getWrapperTypeInfo() const override;

    void Trace(V8GCMarkingVisitor *visitor) override;

    static CanvasRenderingContext2D *ToImpl(v8::Local<v8::Object> object) {
        return ToScriptWrappable(object)->ToImpl<V8CanvasRenderingContext>()->m_context.get();
    }

private:

    friend class V8Canvas;

    std::shared_ptr<CanvasRenderingContext2D> m_context;

    static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context);

    static bool hasInstance(v8::Local<v8::Value> value, v8::Isolate *isolate);

    static void InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                v8::Local<v8::FunctionTemplate> interfaceTemplate);

    static void GetFillStyle(v8::Local<v8::String> name,
                             const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetFillStyle(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info);

    static void GetStrokeStyle(v8::Local<v8::String> name,
                               const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetStrokeStyle(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info);

    static void GetShadowColor(v8::Local<v8::String> name,
                               const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetShadowColor(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info);

    static void GetShadowBlur(v8::Local<v8::String> name,
                              const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetShadowBlur(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info);

    static void GetShadowOffsetX(v8::Local<v8::String> name,
                                 const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetShadowOffsetX(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                                 const v8::PropertyCallbackInfo<void> &info);

    static void GetShadowOffsetY(v8::Local<v8::String> name,
                                 const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetShadowOffsetY(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                                 const v8::PropertyCallbackInfo<void> &info);

    static void GetLineWidth(v8::Local<v8::String> name,
                             const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetLineWidth(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info);

    static void GetLineCap(v8::Local<v8::String> name,
                           const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetLineCap(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info);

    static void GetLineJoin(v8::Local<v8::String> name,
                            const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetLineJoin(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info);

    static void GetLineDashOffset(v8::Local<v8::String> name,
                                  const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetLineDashOffset(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                                  const v8::PropertyCallbackInfo<void> &info);

    static void GetMiterLimit(v8::Local<v8::String> name,
                              const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetMiterLimit(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info);

    static void GetGlobalAlpha(v8::Local<v8::String> name,
                               const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetGlobalAlpha(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info);

    static void GetGlobalCompositeOperation(v8::Local<v8::String> name,
                                            const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetGlobalCompositeOperation(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                                            const v8::PropertyCallbackInfo<void> &info);

    static void GetFont(v8::Local<v8::String> name,
                        const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetFont(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info);

    static void GetTextAlign(v8::Local<v8::String> name,
                             const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetTextAlign(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info);

    static void GetTextBaseline(v8::Local<v8::String> name,
                                const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetTextBaseline(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                                const v8::PropertyCallbackInfo<void> &info);

    static void BeginPath(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Stroke(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Fill(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void MoveTo(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void ClosePath(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void LineTo(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Clip(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void QuadraticCurveTo(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void BezierCurveTo(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Arc(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void ArcTo(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void IsPointInPath(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Rect(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void FillRect(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void StrokeRect(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void ClearRect(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Scale(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Rotate(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Transform(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Translate(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void SetTransform(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void SetLineDash(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void FillText(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void StrokeText(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void MeasureText(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void CreateLinearGradient(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void CreateRadialGradient(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void CreatePattern(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void DrawImage(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void CreateImageData(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void GetImageData(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void PutImageData(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Save(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void Restore(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void CreateEvent(const v8::FunctionCallbackInfo<v8::Value> &args);

    static void ToDataURL(const v8::FunctionCallbackInfo<v8::Value> &args);
};
}


#endif //ATOMGRAPHICS_V8CANVASCONTEXT_H
