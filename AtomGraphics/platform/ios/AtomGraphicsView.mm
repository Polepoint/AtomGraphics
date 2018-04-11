//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGraphicsView.h"
#import "AtomPainter_iOSCoreGraphic.h"
#import "AtomGraphics.h"
#import "AtomCanvasNode.h"
#import "AtomGCanvasNode.h"
#import "AtomJavaScriptCore.h"

using namespace AtomGraphics;

@implementation AtomGraphicsView {
    GCanvasNode *_node;
//    NSTimer *_animationTimer;
    AtomJavaScriptCore *_javaScriptCore;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.contentMode = UIViewContentModeRedraw;
        [self initNode];
        _javaScriptCore = [[AtomJavaScriptCore alloc] initWithCanvasNode:_node];
    }

    return self;
}


- (void)initNode {
    _node = new GCanvasNode((__bridge void *) self);
    _node->setPosition(Vec2(0, 0));
    _node->setContentSize(AtomGraphics::Size(350, 600));
//    CanvasContext2d *ctx = _node->getContext2d();
//    ctx->setFillStyle(Color4F::RED);
//    __block float inc = 0;
//    _animationTimer = [NSTimer timerWithTimeInterval:0.05 repeats:YES block:^(NSTimer *timer) {
//        ctx->beginPath();
//        float r = sin(inc) * 10 + 100;
//        ctx->arc(150, 400, r, 0, 2 * M_PI, 0);
//        inc += 0.1 * 3.14;
//        ctx->stroke();
//        ctx->fill();
//        [self setNeedsDisplay];
//    }];
//
//    [[NSRunLoop currentRunLoop] addTimer:_animationTimer forMode:NSDefaultRunLoopMode];
}


- (void)didMoveToWindow {
    [super didMoveToWindow];
    [_javaScriptCore runScriptFile:[[NSBundle mainBundle] pathForResource:@"main" ofType:@"js"]];
}


//- (void)layoutSubviews {
//    [super layoutSubviews];
//    _node->setDirty(true);
//}
//
//
//- (void)drawRect:(CGRect)rect {
//    [super drawRect:rect];
//    CGContextRef context = UIGraphicsGetCurrentContext();
//    GraphicsContext *graphicsContext = new GraphicsContext(context);
//    Painter *painter = new Painter_iOSCoreGraphic(graphicsContext);
//    if (_node->dirty()) {
//        _node->draw(graphicsContext, painter);
//    }
//}


@end