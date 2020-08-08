//
// Created by neo on 2020/4/22.
//

#ifndef ATOMGRAPHICS_JSIMAGE_H
#define ATOMGRAPHICS_JSIMAGE_H


#import <JavaScriptCore/JavaScriptCore.h>
#import "ImageBitmap.h"
#import "ImageLoader.h"

namespace AtomGraphics {

class JSImage {

public:

    static JSObjectRef ImageConstructor(JSContextRef context);

    static JSObjectRef CreateImageJSObject(JSContextRef context, std::unique_ptr<ImageBitmap> image);

    JSImage(std::unique_ptr<ImageBitmap> image);

private:

    static JSClassRef s_JSClassInfo;

    std::shared_ptr<ImageBitmap> m_image;

    static JSClassRef classInfo();

    static bool SetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef *exception);

    static JSValueRef GetPropertyCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception);

    static void GetPropertyNamesCallback(JSContextRef ctx, JSObjectRef object, JSPropertyNameAccumulatorRef propertyNames);

    static void FinalizeCallback(JSObjectRef object);

    static JSObjectRef CallAsConstructorCallback(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount,
                                                 const JSValueRef arguments[], JSValueRef *exception);

};

}


#endif //ATOMGRAPHICS_JSIMAGE_H
