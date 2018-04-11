//
// Created by neo on 2018/4/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AtomCanvasNode.h"


using namespace AtomGraphics;

@interface AtomJavaScriptCore : NSObject

- (instancetype)initWithCanvasNode:(CanvasNode *)canvasNode;

- (void)runScriptFile:(NSString *)path;

@end