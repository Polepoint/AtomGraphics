//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "CanvasNode.h"

@class JSCanvasContext;

@protocol JSCanvasExport <JSExport>

@property(nonatomic, assign) int width;

@property(nonatomic, assign) int height;

@property(nonatomic, assign, readonly) int clientWidth;

@property(nonatomic, assign, readonly) int clientHeight;

@property(nonatomic, readonly) JSValue *style;

- (JSCanvasContext *)getContext:(NSString *)contentType;

- (void)setAttribute:(NSString *)attrName :(JSValue *)value;

@end

@interface JSCanvas : NSObject <JSCanvasExport>

+ (instancetype)wrapCanvasNode:(std::shared_ptr<AtomGraphics::Node>)node;

- (instancetype)initWithCanvasNode:(std::shared_ptr<AtomGraphics::Node>)node;

- (AtomGraphics::CanvasNode *)getCanvasNode;

@end