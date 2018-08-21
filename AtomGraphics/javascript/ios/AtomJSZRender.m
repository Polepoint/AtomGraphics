//
// Created by neo on 2018/4/26.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomJSZRender.h"


@implementation AtomJSZRender {

}

+ (void)initJSZRender:(JSContext *)context {
    NSString *jsCoreFilePath = [[NSBundle mainBundle] pathForResource:@"zrender" ofType:@"js"];
    [context evaluateScript:[NSString stringWithContentsOfFile:jsCoreFilePath]];
}

@end