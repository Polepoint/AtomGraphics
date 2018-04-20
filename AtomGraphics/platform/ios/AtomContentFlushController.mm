//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomContentFlushController.h"
#import <QuartzCore/CADisplayLink.h>
#import <Foundation/Foundation.h>

@interface AtomViewContentDisplayLinkHandler : NSObject

@end

@implementation AtomViewContentDisplayLinkHandler {
    AtomGraphics::AtomContentFlushController *_contentProxy;
    CADisplayLink *_displayLink;
}

- (instancetype)initWithContentProxy:(AtomGraphics::AtomContentFlushController *)contentProxy {
    self = [super init];
    if (self) {
        _contentProxy = contentProxy;
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayLinkFired:)];
        [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
        _displayLink.paused = YES;
        if (@available(iOS 10, *))
            _displayLink.preferredFramesPerSecond = 60;
    }

    return self;
}

- (void)displayLinkFired:(CADisplayLink *)sender {
    _contentProxy->didRefreshDisplay();
}

- (void)schedule {
    _displayLink.paused = NO;
}

- (void)pause {
    _displayLink.paused = YES;
}

@end

namespace AtomGraphics {

    AtomContentFlushController::AtomContentFlushController() {
        _commitQueue = dispatch_queue_create("neo.AtomGraphics.CommitQueue", nullptr);
//        _flushTimer = new Timer(this, this->flushLayer);
    }

    void AtomContentFlushController::didReceiveMessage() {

    }

    void AtomContentFlushController::didRefreshDisplay() {
        if (_didUpdateMessageState != NeedsDidUpdate) {
            _didUpdateMessageState = MissedCommit;
            [displayLinkHandler() pause];
            return;
        }
        _didUpdateMessageState = DoesNotNeedDidUpdate;
    }


    void AtomContentFlushController::scheduleLayerFlush() {
//        if ([_flushTimer isValid]) {
//            return;
//        }

        // TODO: need flush threshold control ?
        flushLayer();
    }

    void AtomContentFlushController::flushLayer() {
        AtomContentFlushController *thisRef = this;
        if (!_backingStoreToFlush) {
            return;
        }
        AtomLayerBackingStore *backingStore = _backingStoreToFlush;
        dispatch_async(_commitQueue, [thisRef, backingStore] {
            backingStore->flush();
            dispatch_async(dispatch_get_main_queue(), [thisRef] {
                thisRef->commitLayerContent();
            });
        });
    }

    void AtomContentFlushController::commitLayerContent() {
        _backingStoreToFlush->applyBackingStoreToLayer(_contentLayer);
        if (std::exchange(_didUpdateMessageState, NeedsDidUpdate) == MissedCommit)
            didRefreshDisplay();
        [displayLinkHandler() schedule];
    }

    AtomViewContentDisplayLinkHandler *AtomContentFlushController::displayLinkHandler() {
        if (!_displayLinkHandler)
            _displayLinkHandler = [[AtomViewContentDisplayLinkHandler alloc] initWithContentProxy:this];

        return _displayLinkHandler;
    }

    void AtomContentFlushController::setBackingStoreToFlush(AtomLayerBackingStore *backingStoreToFlush) {
        _backingStoreToFlush = backingStoreToFlush;
    }

    void AtomContentFlushController::setContentLayer(CALayer *contentLayer) {
        _contentLayer = contentLayer;
    }
}



