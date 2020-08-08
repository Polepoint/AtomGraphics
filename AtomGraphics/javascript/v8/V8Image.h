//
// Created by neo on 2020/5/3.
//

#ifndef ATGPROJECT_V8IMAGE_H
#define ATGPROJECT_V8IMAGE_H

#include <v8/v8.h>
#include "node/canvas/EventSender.h"
#include "node/canvas/ImageBitmap.h"
#include "javascript/android/JSPageScope.h"
#include "V8GCMarkingVisitor.h"
#include "ScriptWrappable.h"

namespace AtomGraphics {

class V8Image;

struct JSImageOnLoadEventListener : public EventListener {

    JSImageOnLoadEventListener(ImageBitmap *image);

    ~JSImageOnLoadEventListener();

    void onEvent(EventType type, Element *image) override;

    JS::JSPageScope *m_creatorPageScope;
    v8::Isolate *m_isolate;
    v8::Global<v8::Function> m_onLoadFunction;
    uint32_t m_bytes{0};
};

struct JSOnPageDestroyEventListener : public EventListener {

    JSOnPageDestroyEventListener(V8Image *image, JS::JSPageScope *creatorPageScope);

    void onEvent(EventType type, Element *element) override;

    V8Image *m_image;
    JS::JSPageScope *m_pageScope;
};

class V8Image : public ScriptWrappable {

public:

    V8Image();

    ~V8Image();

    const WrapperTypeInfo *getWrapperTypeInfo() const override;

    static const WrapperTypeInfo wrapperTypeInfo;

    static ImageBitmap *ToImpl(v8::Local<v8::Object> object) {
        if (CheckWrapperInfoMatch<V8Image>(object)) {
            return ToScriptWrappable(object)->ToImpl<V8Image>()->m_image.get();
        }

        return nullptr;
    }

    void Trace(V8GCMarkingVisitor *visitor) override;

private:

    std::shared_ptr<ImageBitmap> m_image;
    std::unique_ptr<JSImageOnLoadEventListener> m_onLoadEventListener;
    std::unique_ptr<JSOnPageDestroyEventListener> m_onPageDestroyEventListener;


    static void constructorCallback(const v8::FunctionCallbackInfo<v8::Value> &info);

    static void constructor(const v8::FunctionCallbackInfo<v8::Value> &info);

    static v8::Local<v8::FunctionTemplate>
    domTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context);

    static void InstallTemplate(v8::Isolate *isolate, v8::Local<v8::Context> context,
                                v8::Local<v8::FunctionTemplate> interfaceTemplate);

    static void GetSrc(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void SetSrc(v8::Local<v8::String> name, v8::Local<v8::Value> value,
                       const v8::PropertyCallbackInfo<void> &info);

    static void
    GetWidth(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void
    GetHeight(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void
    GetOnLoad(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void
    SetOnLoad(v8::Local<v8::String> name, v8::Local<v8::Value> value,
              const v8::PropertyCallbackInfo<void> &info);

};

}


#endif //ATGPROJECT_V8IMAGE_H
