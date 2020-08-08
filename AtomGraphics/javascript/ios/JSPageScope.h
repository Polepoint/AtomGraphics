//
// Created by neo on 2018/4/26.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef void (^ScopeHandler)(id data);

@interface JSPageScope : NSObject

@property(nonatomic, assign, readonly) long pageID;

+ (JSPageScope *)createPageJSScopeWithPageID:(long)pageID;

- (void)evaluateScript:(NSString *)script;

- (void)callJSHandler:(NSString *)handlerName parameters:(id)params;

- (void)registerNativeHandler:(NSString *)handlerName handlerCallback:(ScopeHandler)callback;

- (void)destroy;

@end