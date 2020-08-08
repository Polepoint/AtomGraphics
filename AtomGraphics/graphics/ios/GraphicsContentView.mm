//
// Created by neo on 2018/11/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsContentView.h"
#import "CompositingManager.h"
#import "CompositingView.h"
#import "CanvasNodeCG.h"
#import "PlatformTileLayer.h"
#import "TransactionApplier.h"
#import "GraphicsThread.h"
#import "GraphicsLayer.h"
#import "ContentViewProxy.h"
#import "ContentViewUpdateDispatcher.h"

using namespace AtomGraphics;

@interface GraphicsContentView ()

@property(nonatomic, strong) CompositingManager *compositingManager;

@property(atomic, assign) CGRect viewRect;

@property(atomic, assign) CGRect pageBounds;

@property(atomic, assign) float transformScale;

@end

@implementation GraphicsContentView {
    ContentViewProxy *_contentViewProxy;
    bool _didFinishFirstTransaction;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {

    }

    return self;
}

- (void)applyTransaction:(AtomGraphics::Transaction *)transaction {
    if (!_compositingManager) {
        _compositingManager = [CompositingManager new];
        long pageID = _contentViewProxy->getRootLayer()->getPlatformLayer()->getLayerID();
        CompositingView *compositingView = [_compositingManager createViewWithID:pageID];
        [self addSubview:compositingView];
    }

    TransactionApplier::applyTransaction(transaction, _compositingManager);
    [self _finishApplyTransaction];
}


- (void)didUpdateVisibleRect:(const CGRect &)rect pageBounds:(const CGRect &)bounds transformScale:(float)transformScale {
    _viewRect = rect;
    _pageBounds = bounds;
    _transformScale = transformScale;
    GraphicsContentView *selfRef = self;
    GraphicsThread::DispatchOnGraphicsQueue([selfRef]() {
        if (!selfRef->_contentViewProxy) {
            selfRef->_contentViewProxy = new ContentViewProxy;
            [selfRef _finishInitializeOnGraphicsThread];
        }

        selfRef->_contentViewProxy->getPage()->updateVisibleContentRects(GraphicsPageViewInfo(
                selfRef.pageBounds,
                selfRef.viewRect,
                Device::DeviceScaleFactor(),
                selfRef.transformScale));
    });
}

- (void)_finishInitializeOnGraphicsThread {
    _contentViewProxy->getRootLayer()->getPlatformLayer()->setNeedsDisplay();
    [[ContentViewUpdateDispatcher singleton] registerContentView:self withPageID:_contentViewProxy->getPage()->pageID()];
}

- (void)_finishApplyTransaction {
    if (!_didFinishFirstTransaction) {
        _didFinishFirstTransaction = true;
        long pageID = _contentViewProxy->getPage()->pageID();
        [_delegate didFinishInitialization:ContentViewInfo(pageID)];
    }
}

- (void)destroy {
    [[ContentViewUpdateDispatcher singleton] unregisterContentView:_contentViewProxy->getPage()->pageID()];
    GraphicsContentView *selfRef = self;
    GraphicsThread::DispatchOnGraphicsQueue([selfRef]() {
        if (selfRef->_contentViewProxy) {
            selfRef->_contentViewProxy->destroy();
        }
    });
}

@end