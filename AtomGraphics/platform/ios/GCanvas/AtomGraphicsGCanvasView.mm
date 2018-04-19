//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsGCanvasView.h"
#import "AtomGraphicsGCanvasModule.h"
#import "AtomGCanvasContext2d.h"
#import "AtomGCanvasNode.h"
#import "AtomGraphicsView.h"

@interface AtomGraphicsGCanvasView ()

@property(nonatomic, copy) NSString *componentID;

@end


@implementation AtomGraphicsGCanvasView {

}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (!_glkview) {
        _glkview = [[GLKView alloc] initWithFrame:self.bounds];
        _glkview.enableSetNeedsDisplay = YES;
        _glkview.userInteractionEnabled = YES;
        _glkview.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        _glkview.backgroundColor = [UIColor clearColor];
        _glkview.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        [self addSubview:_glkview];
        _componentID = [[NSUUID UUID] UUIDString];
    }

    _componetFrame = frame;
    _needChangeEAGLContenxt = YES;

    return self;
}

- (void)setFrame:(CGRect)frame {
    super.frame = frame;
    _componetFrame = frame;
    _needChangeEAGLContenxt = YES;
}

- (void)removeFromSuperview {
    [super removeFromSuperview];
    [[AtomGraphicsGCanvasModule sharedModule] unregisterCanvasView:_componentID];
}

- (void)willMoveToSuperview:(nullable UIView *)newSuperview {
    [super willMoveToSuperview:newSuperview];
    if (newSuperview) {
        [[AtomGraphicsGCanvasModule sharedModule] registerCanvasView:self];
    }
}


#pragma mark - GCanvasViewProtocol Methods

- (CGFloat)devicePixelRatio {
    return [UIScreen mainScreen].nativeScale;
}

- (NSString *)componentId {
    return _componentID;
}


@end


namespace AtomGraphics {

    GCanvasNode::GCanvasNode(const void *container) : CanvasNode(), _gcanvasContext2d(new GCanvasContext2d()) {
        AtomGraphicsView *rootView = (__bridge AtomGraphicsView *) container;
        AtomGraphicsGCanvasView *gCanvasView = [[AtomGraphicsGCanvasView alloc] initWithFrame:rootView.bounds];
        _componentId = gCanvasView.componentId.cString;
        _gcanvasContext2d->setComponentID(_componentId);
        [rootView addSubview:gCanvasView];
    }

    void GCanvasNode::setPosition(const Vec2 &position) {
        Node::setPosition(position);

        AtomGraphicsGCanvasView *gCanvasView = [[AtomGraphicsGCanvasModule sharedModule] getCanvasView:_componentId.c_str()];
        CGRect oldFrame = gCanvasView.frame;
        gCanvasView.frame = CGRectMake(position.x, position.y, oldFrame.size.width, oldFrame.size.height);
    }


    void GCanvasNode::setContentSize(const AtomGraphics::Size &contentSize) {
        Node::setContentSize(contentSize);

        AtomGraphicsGCanvasView *gCanvasView = [[AtomGraphicsGCanvasModule sharedModule] getCanvasView:_componentId.c_str()];
        CGRect oldFrame = gCanvasView.frame;
        gCanvasView.frame = CGRectMake(oldFrame.origin.x, oldFrame.origin.y, contentSize.width, contentSize.height);
    }

    void GCanvasNode::setContextType(int type) {
        [[AtomGraphicsGCanvasModule sharedModule] setContextType:type componentID:[NSString stringWithCString:_componentId.c_str()]];
    }

    void GCanvasContext2d::addCommand(const std::string &command) {
        [[AtomGraphicsGCanvasModule sharedModule] addCommand:command componentID:_componentId];
    }

    void *GCanvasContext2d::executeCommand(const std::string &command) {
        return nullptr;
    }
}