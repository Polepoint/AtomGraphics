//
// Created by neo on 2019-09-13.
//

#include <v8/v8.h>
#include "graphics/GraphicsPageManager.h"
#include "graphics/GraphicsPage.h"
#include "graphics/GraphicsLayer.h"
#include "node/canvas/android/skia/CanvasNodeSkia.h"
#include "V8JSDocument.h"
#include "V8Canvas.h"

namespace AtomGraphics {

namespace JS {

void V8JSDocument::SetupDocument(v8::Isolate *isolate, v8::Local<v8::Object> global) {
    v8::Local<v8::Object> document = v8::Object::New(isolate);
    global->Set(v8::String::NewFromUtf8(isolate, "document"), document);

    document->Set(v8::String::NewFromUtf8(isolate, "createElement", v8::NewStringType::kNormal).ToLocalChecked(),
                  v8::Function::New(isolate, CreateElement));

    document->Set(v8::String::NewFromUtf8(isolate, "getElementById", v8::NewStringType::kNormal).ToLocalChecked(),
                  v8::Function::New(isolate, GetElementById));
}

void V8JSDocument::CreateElement(const v8::FunctionCallbackInfo<v8::Value> &info) {
    if (info.Length() < 1) {
        return;
    }

    v8::Local<v8::Value> arg0 = info[0];
    if (!arg0->IsString()) {
        return;
    }

    v8::MaybeLocal<v8::String> stringLocalValue = info[0]->ToString(info.GetIsolate()->GetCallingContext());
    if (stringLocalValue.IsEmpty()) {
        return;
    }

    v8::Local<v8::String> stringValue = stringLocalValue.ToLocalChecked();
    v8::String::Utf8Value utf8_value(info.GetIsolate(), stringValue);

    if (strcasecmp(*utf8_value, "canvas") != 0) {
        return;
    }

    auto createNode = std::shared_ptr<Node>(new CanvasNodeSkia());
    V8Canvas *canvas = V8Canvas::CreateCanvasWrappableFromCache(createNode);
    info.GetReturnValue().Set(canvas->Wrap(info.GetIsolate(), info.Holder()->CreationContext()));
}

void V8JSDocument::GetElementById(const v8::FunctionCallbackInfo<v8::Value> &info) {
    if (info.Length() < 1) {
        return;
    }

    std::shared_ptr<Node> rootNode;

    v8::Maybe<double> doubleValue = info[0]->NumberValue(info.GetIsolate()->GetCallingContext());
    if (doubleValue.IsNothing()) {
        return;
    }

    GraphicsPage *page = GraphicsPageManager::getPageByID(static_cast<long>(doubleValue.ToChecked()));
    if (page) {
        rootNode = std::move(page->rootLayer()->getRootNode());
        if (!rootNode) {
            rootNode = std::make_shared<CanvasNodeSkia>();
            page->rootLayer()->setRootNode(rootNode);
        }
    }

    if (!rootNode) {
        return;
    }

    V8Canvas *canvas = V8Canvas::CreateCanvasWrappableFromCache(rootNode);
    info.GetReturnValue().Set(canvas->Wrap(info.GetIsolate(), info.Holder()->CreationContext()));
}

}

}
