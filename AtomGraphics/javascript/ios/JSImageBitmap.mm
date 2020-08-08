//
// Created by neo on 2018/12/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "JSImageBitmap.h"
#import "JSPageScope.h"
#import "JSPageScopeTracer.h"
#import "EventSender.h"

namespace AtomGraphics {

struct JSImageOnLoadEventListener : public EventListener {

    JSImageOnLoadEventListener(JSValue *onLoadedCallback, ImageBitmap *image)
            : jsOnLoadCallback(onLoadedCallback) {
        EventSender::sender()->listenToEvent(EventTypeOnLoad, this, image);
    }

    ~JSImageOnLoadEventListener() {
        EventSender::sender()->removeListener(EventTypeOnLoad, this);
    }

    void onEvent(EventType type, Element *image) override {
        if (type == EventTypeOnLoad) {
            [jsOnLoadCallback callWithArguments:@[]];
        }
    }

    JSValue *jsOnLoadCallback;
};

struct JSOnPageDestroyEventListener : public EventListener {

    JSOnPageDestroyEventListener(JSImageBitmap *image, JSPageScope *creatorPageScope)
            : _image(image),
              _pageScope(creatorPageScope) {
        EventSender::sender()->listenToEvent(EventTypePageScopeWillBeDestroyed, this, nullptr);
    }

    void onEvent(EventType type, Element *element) override {
        if (JS::JSPageScopeTracer::CurrentPageScope() == _pageScope) {
            _pageScope = nil;
            [_image destroy];
            _image = nil;
            EventSender::sender()->removeListener(EventTypePageScopeWillBeDestroyed, this);
        }
    }

    JSImageBitmap *_image;
    JSPageScope *_pageScope;
};

}

using namespace AtomGraphics;

@implementation JSImageBitmap {
    std::shared_ptr<ImageBitmap> _canvasImage;
    std::unique_ptr<JSImageOnLoadEventListener> _onLoadEventListener;
    std::unique_ptr<JSOnPageDestroyEventListener> _onPageDestroyEventListener;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _canvasImage.reset(new ImageBitmap);
        _onPageDestroyEventListener.reset(new JSOnPageDestroyEventListener(self, JS::JSPageScopeTracer::CurrentPageScope()));
    }
    return self;
}

- (ImageBitmap *)getImageBitmap {
    return _canvasImage.get();
}

- (void)setWidth:(CGFloat)width {
    _canvasImage->setWidth(static_cast<float>(width));
}

- (CGFloat)width {
    return _canvasImage->width();
}

- (void)setHeight:(CGFloat)height {
    _canvasImage->setHeight(static_cast<float>(height));
}

- (CGFloat)height {
    return _canvasImage->height();
}

- (void)setSrc:(NSString *)src {
    std::string srcString = [src UTF8String];
    if (_canvasImage->getSrc() != srcString) {
        _canvasImage->setSrc(srcString);
        _canvasImage->imageLoader()->updateFromElement();
    }
}

- (NSString *)src {
    std::string srcString = _canvasImage->getSrc();
    NSString *textValue = [NSString stringWithCString:srcString.data() encoding:NSUTF8StringEncoding];
    return textValue;
}

- (void)setOnload:(JSValue *)onload {
    if (onload.isNull && onload.isUndefined) {
        _onLoadEventListener = nullptr;
    } else {
        _onLoadEventListener = std::make_unique<JSImageOnLoadEventListener>(onload, _canvasImage.get());
    }
}

- (JSValue *)onload {
    if (_onLoadEventListener) {
        return _onLoadEventListener->jsOnLoadCallback;
    }

    return nullptr;
}

- (void)destroy {
    _onLoadEventListener.reset();
}

@end