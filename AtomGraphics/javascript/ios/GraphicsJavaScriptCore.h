//
// Created by neo on 2018/4/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JSContext.h>

@interface GraphicsJavaScriptCore : NSObject

@property(nonatomic, readonly, strong) JSContext *context;

+ (GraphicsJavaScriptCore *)sharedJavaScriptCore;

- (void)runScriptFile:(NSURL *)sourceURL;

- (void)runScript:(NSString *)script;

- (void)runScript:(NSString *)script debugSource:(NSURL *)sourceUrl;

- (void)reset;

@end