//
// Created by neo on 2018/12/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>

@class JSPageScope;

typedef enum {
    EvaluateScript, CallJSHandler, RegisterNativeHandler
} CommandType;

@interface PageScopeCommandQueue : NSObject

- (void)addCommand:(CommandType)type data1:(id)data1 data2:(id)data2;

- (void)flushCommandToPageScope:(JSPageScope *)scope;

- (BOOL)empty;

- (void)clear;

@end