//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGraphicsView.h"
#import "AtomNode.h"
#import "AtomPainter_iOSCoreGraphic.h"
#import "AtomGraphics.h"

using namespace AtomGraphics;

@implementation AtomGraphicsView {
    Node *_node;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.contentMode = UIViewContentModeRedraw;
        [self initNode];
    }

    return self;
}


- (void)initNode {
    _node = new Node();
    _node->setBackgroundColor(Color4F::RED);
    _node->setPosition(Vec2(20, 100));
    _node->setContentSize(AtomGraphics::Size(200, 40));
}

- (void)layoutSubviews {
    [super layoutSubviews];
    _node->setDirty(true);
}


- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    CGContextRef context = UIGraphicsGetCurrentContext();
    GraphicsContext *graphicsContext = new GraphicsContext(context);
    Painter *painter = new Painter_iOSCoreGraphic(graphicsContext);
    if (_node->dirty()) {
        _node->draw(graphicsContext, painter);
    }
}


@end