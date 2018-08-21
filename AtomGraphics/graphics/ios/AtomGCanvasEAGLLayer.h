//
// Created by neo on 2018/7/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/CAEAGLLayer.h>
#import <GCanvas/GCanvasPlugin.h>

@interface AtomGCanvasEAGLLayer : CAEAGLLayer

@property(nonatomic, strong) GCanvasPlugin *plugin;

- (void)renderCommand:(NSString *)command;

- (void)resizeLayerBuffer;

@end