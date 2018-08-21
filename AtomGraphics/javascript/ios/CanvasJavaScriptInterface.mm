//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "CanvasJavaScriptInterface.h"
#import "CanvasContextJavaScriptInterface.h"


using namespace AtomGraphics;

@implementation CanvasJavaScriptInterface {
    CanvasNode *_canvasNode;
}

- (instancetype)initWithCanvasNode:(CanvasNode *)canvasNode {
    self = [super init];
    if (self) {
        _canvasNode = canvasNode;
    }

    return self;
}

- (int)width {
    if (_canvasNode) {
        return _canvasNode->getFrame().size.width;
    } else {
        return 0;
    }
}

- (void)setWidth:(int)width {
    if (_canvasNode) {
        AtomGraphics::Rect frame = _canvasNode->getFrame();
        _canvasNode->setFrame(AtomGraphics::Rect(frame.origin.x, frame.origin.y, width, frame.size.height));
    }
}

- (int)height {
    if (_canvasNode) {
        return _canvasNode->getFrame().size.height;
    } else {
        return 0;
    }
}

- (void)setHeight:(int)height {
    if (_canvasNode) {
        AtomGraphics::Rect frame = _canvasNode->getFrame();
        _canvasNode->setFrame(AtomGraphics::Rect(frame.origin.x, frame.origin.y, frame.size.width, height));
    }
}


- (CanvasContextJavaScriptInterface *)getContext:(NSString *)contentType {
    if (_canvasNode) {
        return [[CanvasContextJavaScriptInterface alloc] initWithCanvasContext:_canvasNode->getContext2d()];
    } else {
        return nil;
    }
}

- (CanvasNode *)getCanvasNode {
    return _canvasNode;
}

@end