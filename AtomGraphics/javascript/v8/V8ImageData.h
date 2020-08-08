//
// Created by neo on 2019/1/30.
//

#ifndef ATOMGRAPHICS_V8IMAGEDATA_H
#define ATOMGRAPHICS_V8IMAGEDATA_H

#include <v8/v8.h>
#include "graphics/ImageData.h"
#include "ScriptWrappable.h"

namespace AtomGraphics {

class V8ImageData : public ScriptWrappable {

public:

    V8ImageData(scoped_refptr<ImageData> imageData);

    ~V8ImageData();

    const WrapperTypeInfo *getWrapperTypeInfo() const override;

    static const WrapperTypeInfo wrapperTypeInfo;

    static ImageData *ToImpl(v8::Local<v8::Object> object) {
        return ToScriptWrappable(object)->ToImpl<V8ImageData>()->m_imageData.get();
    }

    void Trace(V8GCMarkingVisitor *visitor) override;


private:

    scoped_refptr<ImageData> m_imageData;
    v8::Global<v8::Uint8ClampedArray> m_data;

    static void constructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void constructor(const v8::FunctionCallbackInfo<v8::Value> &info);

    static v8::Local<v8::FunctionTemplate> domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context);

    static void InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                v8::Local<v8::FunctionTemplate> interfaceTemplate);

    static void GetData(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void GetWidth(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void GetHeight(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);
};

}

#endif //ATOMGRAPHICS_V8IMAGEDATA_H
