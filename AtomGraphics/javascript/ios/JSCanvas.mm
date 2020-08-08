//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "JSCanvas.h"
#import "JSCanvasContext.h"
#import "JSCSSStyle.h"
#import "CSSToken.h"
#import "CSSParser.h"


using namespace AtomGraphics;

static NSCache *sCanvasWrapperCache;

@interface JSCanvas ()

@property(nonatomic, strong) JSCanvasContext *context;

@end

@implementation JSCanvas {
    std::shared_ptr<Node> _node;
    CanvasNode *_canvasNode;
    JSValue *_cssStyle;
}

+ (instancetype)wrapCanvasNode:(std::shared_ptr<Node>)node {

    if (!sCanvasWrapperCache) {
        sCanvasWrapperCache = [NSCache new];
    }

    NSValue *entry = [sCanvasWrapperCache objectForKey:@(reinterpret_cast<intptr_t>(node.get()))];
    JSCanvas *canvas;
    if (entry) {
        canvas = [entry nonretainedObjectValue];
    }

    if (!canvas) {
        canvas = [[JSCanvas alloc] initWithCanvasNode:node];
        [sCanvasWrapperCache setObject:[NSValue valueWithNonretainedObject:canvas] forKey:@(reinterpret_cast<intptr_t>(node.get()))];
    }

    return canvas;
}

- (instancetype)initWithCanvasNode:(std::shared_ptr<Node>)node {
    self = [super init];
    if (self) {
        _node = node;
        _canvasNode = dynamic_cast<CanvasNode *>(node.get());
    }

    return self;
}

- (int)width {
    if (_canvasNode) {
        return static_cast<int>(_canvasNode->contentWidth());
    } else {
        return 0;
    }
}

- (void)setWidth:(int)width {
    if (_canvasNode) {
        _canvasNode->setContentWidth(width);
    }
}

- (int)height {
    if (_canvasNode) {
        return static_cast<int>(_canvasNode->contentHeight());
    } else {
        return 0;
    }
}

- (void)setHeight:(int)height {
    if (_canvasNode) {
        _canvasNode->setContentHeight(height);
    }
}


- (int)clientWidth {
    if (_canvasNode) {
        return static_cast<int>(_canvasNode->getFrame().width());
    } else {
        return 0;
    }
}


- (int)clientHeight {
    if (_canvasNode) {
        return static_cast<int>(_canvasNode->getFrame().height());
    } else {
        return 0;
    }
}

void OnCanvasStylePropertyChanged(const char *propertyName, const CSSStylePropertyValue &value, void *userData) {
    CanvasNode *node = static_cast<CanvasNode *>(userData);
    if (std::strcmp(propertyName, "width") == 0) {
        auto rect = node->getFrame();
        rect.setWidth(static_cast<float>(value.toNumber()));
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "height") == 0) {
        auto rect = node->getFrame();
        rect.setHeight(static_cast<float>(value.toNumber()));
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "left") == 0) {
        auto rect = node->getFrame();
        float maxX = rect.getMaxX();
        rect.origin.x = static_cast<float>(value.toNumber());
        rect.setWidth(maxX - rect.origin.x);
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "top") == 0) {
        auto rect = node->getFrame();
        float maxY = rect.getMaxY();
        rect.origin.y = static_cast<float>(value.toNumber());
        rect.setHeight(maxY - rect.origin.y);
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "right") == 0) {
        auto rect = node->getFrame();
        rect.setWidth(static_cast<float>(value.toNumber() - rect.origin.x));
        node->setFrame(rect);
    } else if (std::strcmp(propertyName, "bottom") == 0) {
        auto rect = node->getFrame();
        rect.setHeight(static_cast<float>(value.toNumber() - rect.origin.y));
        node->setFrame(rect);
    }
}

- (JSValue *)style {
    if (!_cssStyle) {
        JSContext *context = [JSContext currentContext];
        CSSStyleDeclaration *declaration = new CSSStyleDeclaration(OnCanvasStylePropertyChanged, _canvasNode);
        declaration->setProperty("width", CSSStylePropertyValue(_canvasNode->getFrame().width(), CSS_PX));
        declaration->setProperty("height", CSSStylePropertyValue(_canvasNode->getFrame().height(), CSS_PX));
        std::unique_ptr<CSSStyleDeclaration> decl(declaration);
        JSObjectRef cssStyleDeclarationJSObject = JSCSSStyle::CreateCSSStyleDeclarationJSObject([context JSGlobalContextRef], std::move(decl));
        _cssStyle = [JSValue valueWithJSValueRef:cssStyleDeclarationJSObject inContext:context];
    }

    return _cssStyle;
}

- (void)setAttribute:(NSString *)attrName :(JSValue *)value {
    if (!attrName) {
        return;
    }
    if ([value isNumber]) {
        OnCanvasStylePropertyChanged([attrName cString], CSSStylePropertyValue([value toDouble]), _canvasNode);
    } else if ([value isString]) {
        NSString *stringValue = [value toString];
        NSUInteger strLength = [stringValue length];
        if (!strLength) {
            return;
        }
        CSSToken token([stringValue cString], 0, strLength - 1);
        auto propertyValue = CSSParser::tokenToCSSPropertyValue(token);
        if (propertyValue) {
            OnCanvasStylePropertyChanged([attrName cString], *propertyValue, _canvasNode);
        }
    }

}

- (JSCanvasContext *)getContext:(NSString *)contentType {
    if (_canvasNode && !_context) {
        _context = [[JSCanvasContext alloc] initWithCanvasContext:_canvasNode->getContext2d()];
    }

    return _context;
}

- (CanvasNode *)getCanvasNode {
    return _canvasNode;
}

- (void)dealloc {
    [sCanvasWrapperCache removeObjectForKey:@(reinterpret_cast<intptr_t>(_node.get()))];
}


@end