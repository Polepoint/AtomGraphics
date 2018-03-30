//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGraphicsView.h"
#import "AtomNode.h"

using namespace atomgraphics;

@implementation AtomGraphicsView {
    Node *_node;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self initNode];
    }

    return self;
}


- (void)initNode {
    _node = new Node();
}

- (void)drawLayer:(CALayer *)layer inContext:(CGContextRef)ctx {
    [super drawLayer:layer inContext:ctx];
    GraphicsContext *graphicsContext = new GraphicsContext(ctx);
    if (_node->dirty()) {
        Renderer::render(_node, graphicsContext);
    }
}


- (void)renderNode {

}

@end