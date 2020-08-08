//
// Created by neo on 2020/2/18.
// Copyright (c) 2020 neo. All rights reserved.
//

#import "PerformanceMonitor.h"
#import "MonitorView.h"

@interface PerformanceMonitor ()

@property(nonatomic, strong) MonitorView *monitorView;
@property(nonatomic, strong) UIPanGestureRecognizer *panGestureRecognizer;
@property(nonatomic, strong) CADisplayLink *displayLink;

@end

@implementation PerformanceMonitor {
    BOOL _changingMonitorSize;
}

- (void)show {
    _shown = YES;

    UIWindow *keyWindow = [PerformanceMonitor keyWindow];
    [keyWindow addSubview:self.monitorView];
    [keyWindow bringSubviewToFront:self.monitorView];

    _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(updateStatus:)];
    [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
}

- (void)hide {
    _shown = NO;

    if (!_monitorView) {
        return;
    }

    [_monitorView removeFromSuperview];
    [_displayLink removeFromRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
}

+ (UIWindow *)keyWindow {
    NSArray *windows = [[UIApplication sharedApplication] windows];
    for (UIWindow *window in windows) {
        if (window.keyWindow) {
            return window;
        }
    }

    return windows.firstObject;
}


- (void)updateStatus:(CADisplayLink *)displayLink {
    [_monitorView onFrameStep:displayLink.timestamp];
}

- (void)onPan:(UIPanGestureRecognizer *)panGestureRecognizer {
    switch (panGestureRecognizer.state) {
        case UIGestureRecognizerStateBegan: {
            CGPoint locationInView = [panGestureRecognizer locationInView:_monitorView];
            if (fabs(locationInView.x - _monitorView.frame.size.width) < 30
                    && fabs(locationInView.y - _monitorView.frame.size.height) < 30) {
                _changingMonitorSize = YES;
            }
            break;
        }

        case UIGestureRecognizerStateEnded:
        case UIGestureRecognizerStateCancelled:
            _changingMonitorSize = NO;
            return;
        default: {
        }
    }

    if (_changingMonitorSize) {
        CGPoint locationInView = [panGestureRecognizer locationInView:_monitorView];
        _monitorView.frame = (CGRect) {
                _monitorView.frame.origin,
                (CGSize) {
                        locationInView.x,
                        locationInView.y
                }
        };
    } else {
        CGPoint translationInWindow = [panGestureRecognizer translationInView:_monitorView.superview];

        _monitorView.center = CGPointMake(
                _monitorView.center.x + translationInWindow.x,
                _monitorView.center.y + translationInWindow.y
        );

        [panGestureRecognizer setTranslation:CGPointMake(0, 0)
                                      inView:_monitorView.superview];
    }
}

- (UIPanGestureRecognizer *)panGestureRecognizer {
    if (!_panGestureRecognizer) {
        _panGestureRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(onPan:)];
    }
    return _panGestureRecognizer;
}


- (MonitorView *)monitorView {
    if (!_monitorView) {
        _monitorView = [[MonitorView alloc] initWithFrame:CGRectMake(50, 100, 300, 100)];
        [_monitorView addGestureRecognizer:self.panGestureRecognizer];
    }

    return _monitorView;
}


@end