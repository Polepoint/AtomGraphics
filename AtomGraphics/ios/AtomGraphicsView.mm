//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsView.h"
#import "AtomGraphics.h"
#import "GraphicsContentView.h"
#import "PageScopeCommandQueue.h"
#import "GraphicsEnvironment.h"
#import "UIView+ATG.h"
#import "GraphicsConfiguration.h"

using namespace AtomGraphics;

@interface AtomGraphicsView () <GraphicsContentViewDelegate>

@property(nonatomic, strong) GraphicsContentView *contentView;
@property(nonatomic, strong) JSPageScope *pageScope;
@property(nonatomic, strong) PageScopeCommandQueue *pendingCommandQueue;

@end

@implementation AtomGraphicsView {
    BOOL _hasScheduledVisibleRectUpdate;
}

+ (void)initialize {
    [super initialize];
    [GraphicsEnvironment initializeEnvironment];
}


- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        _pendingCommandQueue = [PageScopeCommandQueue new];
        _contentView = [[GraphicsContentView alloc] initWithFrame:self.bounds];
        _contentView.delegate = self;
        _contentView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [self addSubview:_contentView];

        [self _scheduleVisibleContentRectUpdate];
    }

    return self;
}

- (void)dealloc {
    [_contentView destroy];
    [_pageScope destroy];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    [self _scheduleVisibleContentRectUpdate];
}

// UIKit SPI
// Called when some ancestor UIScrollView scrolls.
- (void)_didScroll {
    [self _scheduleVisibleContentRectUpdate];
}

- (void)_scheduleVisibleContentRectUpdate {

    if (_hasScheduledVisibleRectUpdate) {
        return;
    }

    _hasScheduledVisibleRectUpdate = YES;

    //send visible rect to GraphicsLayer

    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^() {
        [weakSelf _addUpdateVisibleContentRectPreCommitHandler];
    });
}

- (void)_addUpdateVisibleContentRectPreCommitHandler {
    [CATransaction setCompletionBlock:^{
        [self _updateVisibleContentRects];
        self->_hasScheduledVisibleRectUpdate = NO;
    }];
}

- (void)_updateVisibleContentRects {

    CGRect visibleRect = [self _visibleContentRect];

    [_contentView didUpdateVisibleRect:visibleRect pageBounds:self.bounds transformScale:[self _screenToContentScale]];
    if ([_delegate respondsToSelector:@selector(graphicsView:viewportDidChange:)]) {
        [_delegate graphicsView:self viewportDidChange:visibleRect];
    }
}

- (CGRect)_visibleContentRect {

    UIView *enclosingView = [self scrollerSuperview];
    if (!enclosingView)
        return self.bounds;

    CGRect exposedRect = [enclosingView convertRect:enclosingView.bounds toView:self];
    return CGRectIntersectsRect(exposedRect, self.bounds) ? CGRectIntersection(exposedRect, self.bounds) : CGRectZero;
}

- (UIView *)_topScroller {
    UIView *view = [self scrollerSuperview];
    while ([view scrollerSuperview]) {
        view = [view scrollerSuperview];
    }

    return view;
}

- (float)_screenToContentScale {

    CGRect unscaledBounds = [self convertRect:self.bounds toView:[self _topScroller]];
    if (unscaledBounds.size.width) {
        return unscaledBounds.size.width / self.bounds.size.width;
    } else if (unscaledBounds.size.height) {
        return unscaledBounds.size.height / self.bounds.size.height;
    }

    return 1;
}

- (void)evaluateScript:(NSString *)script {
    if (!script) {
        return;
    }

    if (_pageScope) {
        [_pageScope evaluateScript:script];
    } else {
        [_pendingCommandQueue addCommand:CommandType::EvaluateScript data1:script data2:nil];
    }
}

- (void)callJSHandler:(NSString *)handlerName {
    [self callJSHandler:handlerName parameters:nil];
}

- (void)callJSHandler:(NSString *)handlerName parameters:(id)params {
    if (!handlerName) {
        return;
    }

    if (_pageScope) {
        [_pageScope callJSHandler:handlerName parameters:params];
    } else {
        [_pendingCommandQueue addCommand:CommandType::CallJSHandler data1:handlerName data2:params];
    }
}

- (void)registerNativeHandler:(NSString *)handlerName handlerCallback:(ScopeHandler)callback {
    if (!handlerName) {
        return;
    }

    if (_pageScope) {
        [_pageScope registerNativeHandler:handlerName handlerCallback:callback];
    } else {
        [_pendingCommandQueue addCommand:CommandType::RegisterNativeHandler data1:handlerName data2:callback];
    }
}

#pragma mark GraphicsContentViewDelegate

- (void)didFinishInitialization:(ContentViewInfo)info {
    _pageScope = [JSPageScope createPageJSScopeWithPageID:info.pageID];
    if (![_pendingCommandQueue empty]) {
        [_pendingCommandQueue flushCommandToPageScope:_pageScope];
    }
}

@end