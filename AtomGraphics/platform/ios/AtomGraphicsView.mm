//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsView.h"
#import "AtomGraphics.h"
#import "AtomCanvasNode.h"
#import "AtomGCanvasNode.h"
#import "AtomJavaScriptCore.h"
#import "PlatformLayerCAEAGL.h"
#import "GraphicsLayerGCanvas.h"

using namespace AtomGraphics;

static GraphicsPageContext *sharedContext = new GraphicsPageContext();

@implementation AtomGraphicsView {
    GraphicsPage *_page;
    GCanvasNode *_node;
    AtomGCanvasEAGLLayer *_glLayer;
}

+ (void)initialize {
    [super initialize];
    AtomGraphics::GraphicsThread::InitGraphicsThread();
}

+ (GraphicsPageContext *)PageContext {
    if (!sharedContext) {
        sharedContext = new GraphicsPageContext();
    }
    return sharedContext;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self initGraphicsPage];
    }
    return self;
}

- (void)initGraphicsPage {
    _glLayer = [AtomGCanvasEAGLLayer layer];
    _glLayer.frame = self.layer.bounds;
    _glLayer.contentsScale = [UIScreen mainScreen].scale;
    [self.layer addSublayer:_glLayer];

    _node = new GCanvasNode();
    _node->setFrame(AtomGraphics::Rect(0, 0, self.bounds.size.width, self.bounds.size.height));
    ((GCanvasContext2D *) _node->getContext2d())->setDevicePixelRatio(_glLayer.contentsScale);

    PlatformLayerCAEAGL *platformLayer = new PlatformLayerCAEAGL(_glLayer);
    GraphicsLayerGCanvas *graphicsLayer = new GraphicsLayerGCanvas(platformLayer, _node);
    if (!sharedContext) {
        sharedContext = new GraphicsPageContext();
    }
    _page = new GraphicsPage(sharedContext, graphicsLayer);
}

- (void)layoutSubviews {
    [super layoutSubviews];
    if (!CGRectEqualToRect(_glLayer.frame, self.layer.bounds)) {
        CGRect bounds = self.layer.bounds;
        _glLayer.frame = bounds;
        _node->setFrame(AtomGraphics::Rect(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height));
        [_glLayer resizeLayerBuffer];
    }
}


- (void)reloadGraphics {
    [[AtomJavaScriptCore javaScriptCoreForContext:sharedContext] runScript:[NSString stringWithFormat:@"redraw(%ld)", _page->pageID()]];
}

- (void)dealloc {
    delete _page;
    delete _node;
}

@end