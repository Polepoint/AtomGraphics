//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AtomGraphicsGCanvasView;


@interface AtomGraphicsGCanvasModule : NSObject


+ (AtomGraphicsGCanvasModule *)sharedModule;

- (void)registerCanvasView:(AtomGraphicsGCanvasView *)gCanvasView;

- (void)unregisterCanvasView:(NSString *)componentID;

- (AtomGraphicsGCanvasView *)getCanvasView:(NSString *)componentID;

- (void)addCommand:(NSString *)command componentID:(NSString *)componentID;

- (void)setContextType:(NSUInteger)type componentID:(NSString *)componentID;
@end