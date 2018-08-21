//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomCanvasNode.h"

@class CanvasContextJavaScriptInterface;

@protocol CanvasJavaScriptInterfaceExport <JSExport>

@property(nonatomic, assign) int width;

@property(nonatomic, assign) int height;


- (CanvasContextJavaScriptInterface *)getContext:(NSString *)contentType;

- (AtomGraphics::CanvasNode *)getCanvasNode;

@end

@interface CanvasJavaScriptInterface : NSObject <CanvasJavaScriptInterfaceExport>

- (instancetype)initWithCanvasNode:(AtomGraphics::CanvasNode *)canvasNode;

@end