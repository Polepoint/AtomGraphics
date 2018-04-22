//
// Created by neo on 2018/4/2.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsCGLayer.h"
#import "AtomLayerBackingStoreCG.h"


@implementation AtomGraphicsCGLayer {
    AtomGraphics::AtomContentFlushController *_flushController;
    AtomGraphics::CanvasNodeCG *_rootNode;
    AtomGraphics::AtomLayerBackingStoreCG *_backingStoreCG;
}


- (instancetype)init {
    self = [super init];
    if (self) {
        _flushController = new AtomGraphics::AtomContentFlushController();
        _flushController->setContentLayer(self);
    }

    return self;
}


- (void)setRootNode:(AtomGraphics::CanvasNodeCG *)rootNode {
    _rootNode = rootNode;
    _rootNode->setPosition(AtomGraphics::Vec2(self.bounds.origin.x, self.bounds.origin.y));
    _rootNode->setContentSize(AtomGraphics::Size(self.bounds.size.width, self.bounds.size.height));
    _backingStoreCG = new AtomGraphics::AtomLayerBackingStoreCG(_rootNode);
    _backingStoreCG->setContentSize(AtomGraphics::Size(self.bounds.size.width, self.bounds.size.height));
    _flushController->setBackingStoreToFlush(_backingStoreCG);
}

- (AtomGraphics::AtomContentFlushController *)getFlushController {
    return _flushController;
}

- (void)setFrame:(CGRect)frame {
    super.frame = frame;
    if(_rootNode){
        _rootNode->setPosition(AtomGraphics::Vec2(self.bounds.origin.x, self.bounds.origin.y));
        _rootNode->setContentSize(AtomGraphics::Size(self.bounds.size.width, self.bounds.size.height));
    }
    if(_backingStoreCG){
        _backingStoreCG->setContentSize(AtomGraphics::Size(self.bounds.size.width, self.bounds.size.height));
    }
}

@end