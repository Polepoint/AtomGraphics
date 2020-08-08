//
// Created by neo on 2019-09-19.
//

#import "JSImageData.h"

using namespace AtomGraphics;

@implementation JSImageData {
    scoped_refptr<ImageData> _imageData;
    JSValue *_data;
}

- (instancetype)initWithCanvasImageData:(scoped_refptr<ImageData>)canvasImageData {
    self = [super init];
    if (self) {
        _imageData = std::move(canvasImageData);
    }
    return self;
}

- (ImageData *)getCanvasImageData {
    return _imageData.get();
}

void TypedArrayBytesDeallocator(void *bytes, void *deallocatorContext) {
    ImageData *image = static_cast<ImageData *>(deallocatorContext);
    image->deref();
}

- (JSValue *)data {
    if (!_data) {
        JSContext *context = [JSContext currentContext];
        JSValueRef *exception = nullptr;
        JSObjectRef object = JSObjectMakeTypedArrayWithBytesNoCopy(
                context.JSGlobalContextRef,
                kJSTypedArrayTypeUint8ClampedArray,
                _imageData->data(),
                _imageData->bytesCount(),
                TypedArrayBytesDeallocator,
                _imageData.get(),
                exception);

        _imageData->ref();
        _data = [JSValue valueWithJSValueRef:object inContext:context];
    }

    return _data;
}

- (CGFloat)width {
    return _imageData->width();
}

- (CGFloat)height {
    return _imageData->height();
}

@end