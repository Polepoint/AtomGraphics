//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGraphicsGCanvasComponent.h"
#import "AtomGraphicsGCanvasView.h"


@implementation AtomGraphicsGCanvasComponent {

}

- (instancetype)initWithCanvasView:(AtomGraphicsGCanvasView *)canvasView {
    self = [super init];
    if (self) {
        _componentID = canvasView.componentId;
        _renderCommands = [NSMutableArray array];
        _gCanvasView = canvasView;
    }

    return self;
}

@end