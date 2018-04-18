//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GCanvas/GCanvasModule.h>
#import <string>

@class AtomGraphicsGCanvasView;


@interface AtomGraphicsGCanvasModule : NSObject

@property(nonatomic, strong) GCanvasModule *gcanvasModule;

+ (AtomGraphicsGCanvasModule *)sharedModule;

- (void)registerCanvasView:(AtomGraphicsGCanvasView *)gCanvasView;

- (void)unregisterCanvasView:(NSString *)componentID;

- (AtomGraphicsGCanvasView *)getCanvasView:(const std::string &)componentID;

- (void)addCommand:(const std::string &)command componentID:(const std::string &)componentID;

- (void)setContextType:(NSUInteger)type componentID:(NSString *)componentID;

@end