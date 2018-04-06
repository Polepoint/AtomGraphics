//
// Created by neo on 2018/4/6.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "FPSLabel.h"

@interface FPSLabel ()

@property(nonatomic, strong) CADisplayLink *displayLink;
@property(nonatomic, strong) UIView *label;

@end

@implementation FPSLabel {
    NSTimeInterval _prevTimeInterval;
    int _frameCount;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(record)];
        [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
        _prevTimeInterval = [[NSDate date] timeIntervalSince1970];
        self.textAlignment = NSTextAlignmentCenter;
        self.font = [UIFont systemFontOfSize:14];
        self.textColor = [UIColor blackColor];
    }

    return self;
}

- (void)record {
    _frameCount++;
    NSTimeInterval currentTimeInterval = [[NSDate date] timeIntervalSince1970];
    if (currentTimeInterval - _prevTimeInterval + 0.016 > 1) {
        _prevTimeInterval = currentTimeInterval;
        self.text = [NSString stringWithFormat:@"FPS: %d", _frameCount];
        _frameCount = 0;
    }
}


@end