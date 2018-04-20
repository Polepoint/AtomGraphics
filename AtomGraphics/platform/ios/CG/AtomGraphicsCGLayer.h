//
// Created by neo on 2018/4/2.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "AtomCanvasNodeCG.h"
#import "AtomContentFlushController.h"

@interface AtomGraphicsCGLayer : CALayer

- (void)setRootNode:(AtomGraphics::CanvasNodeCG *)rootNode;

- (AtomGraphics::AtomContentFlushController *)getFlushController;

@end