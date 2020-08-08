//
// Created by neo on 2020/4/22.
//



#include "JSImage.h"

namespace AtomGraphics {

JSObjectRef JSImage::ImageConstructor(JSContextRef context) {
    return JSObjectMakeConstructor(context, classInfo(), CallAsConstructorCallback);
}

JSObjectRef JSImage::CreateImageJSObject(JSContextRef context, std::unique_ptr<ImageBitmap> image) {
    JSImage *jsImage = new JSImage(std::move(image));
    return JSObjectMake(context, classInfo(), jsImage);
}

JSImage::JSImage(std::unique_ptr<ImageBitmap> image)
        : m_image(std::move(image)) {

}

JSClassRef JSImage::classInfo() {
    if (!s_JSClassInfo) {
        static JSClassDefinition classDefinition;
        classDefinition.className = "HTMLImageElement";
        classDefinition.setProperty = SetPropertyCallback;
        classDefinition.getProperty = GetPropertyCallback;
        classDefinition.getPropertyNames = GetPropertyNamesCallback;
        classDefinition.finalize = FinalizeCallback;
        s_JSClassInfo = JSClassCreate(&classDefinition);
    }

    return s_JSClassInfo;
}

bool JSImage::SetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef *exception) {
    return false;
}

JSValueRef JSImage::GetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception) {
    return nullptr;
}

void JSImage::GetPropertyNamesCallback(JSContextRef ctx, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames) {

}

void JSImage::FinalizeCallback(JSObjectRef object) {

}

JSObjectRef JSImage::CallAsConstructorCallback(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount,
                                               JSValueRef const *arguments, JSValueRef *exception) {
    return CreateImageJSObject(ctx, std::make_unique<ImageBitmap>());
}

}

