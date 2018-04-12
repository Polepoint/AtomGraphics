//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AtomGraphicsGCanvasView;


@interface AtomGraphicsGCanvasComponent : NSObject

@property(nonatomic, copy) NSString *componentID;

@property(nonatomic, weak) AtomGraphicsGCanvasView *gCanvasView;

@property(nonatomic, strong) NSMutableArray *renderCommands;

@property(nonatomic, assign) BOOL isCountingDown;

- (instancetype)initWithCanvasView:(AtomGraphicsGCanvasView *)canvasView;

@end