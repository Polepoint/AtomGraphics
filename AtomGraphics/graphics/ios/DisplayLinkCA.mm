//
// Created by neo on 2018/7/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "DisplayLinkCA.h"

@interface CADisplayLinkHandler : NSObject

@end

@implementation CADisplayLinkHandler {
    AtomGraphics::DisplayLinkCA *_contentProxy;
    CADisplayLink *_displayLink;
}

- (instancetype)initWithContentProxy:(AtomGraphics::DisplayLinkCA *)contentProxy {
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
    _contentProxy->displayLinkFired();
}

- (void)schedule {
    _displayLink.paused = NO;
}

- (void)pause {
    _displayLink.paused = YES;
}

@end


namespace AtomGraphics {

DisplayLinkCA::DisplayLinkCA(AtomGraphics::GraphicsContentFlushController *flushController)
        : DisplayLink(flushController) {
    m_displayLinkHandler = [[CADisplayLinkHandler alloc] initWithContentProxy:this];
}

void DisplayLinkCA::schedule() {
    [m_displayLinkHandler schedule];
}

void DisplayLinkCA::pause() {
    [m_displayLinkHandler pause];
}


}
