//
// Created by neo on 2018/4/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "JSWindow.h"
#import "Timer.h"
#import "ScriptTimerController.h"
#import "ScriptAnimationFrameController.h"
#import "GraphicsContentFlushController.h"
#import "Device.h"
#import "JSDocument.h"
#import "JSPageScopeTracer.h"
#import "JSExecutionChecker.h"

using namespace AtomGraphics;

@implementation JSWindow


+ (void)setupContext:(JSContext *)context {
    NSBundle *bundle = [NSBundle bundleForClass:self.class];
    NSString *jsCoreFilePath = [bundle pathForResource:@"timers" ofType:@"js"];
    [context evaluateScript:[NSString stringWithContentsOfFile:jsCoreFilePath encoding:NSUTF8StringEncoding error:nil]];

//    FIXME: window == context.globalObject ?
//    context.globalObject[@"window"] = context.globalObject;
//    JSValue *window = context.globalObject;

    context[@"setTimeout"] = ^() {
        JSPageScope *pageScope = JS::JSPageScopeTracer::CurrentPageScope();
        NSArray *arguments = [JSContext currentArguments];
        long timerID = -1;
        if (arguments.count > 0) {
            NSArray *callbackArguments = arguments.count > 2 ? [arguments subarrayWithRange:NSMakeRange(2, arguments.count - 2)] : @[];
            long timeout = arguments.count > 1 ? [[arguments[1] toNumber] longValue] : 0;
            timerID = AtomGraphics::ScriptTimerController::CreateTimerID(timeout, false, [pageScope, arguments, callbackArguments]() {
                JS::JSPageScopeTracer tracer(pageScope);
                [JSExecutionChecker checkFunctionExecutionResult:[arguments[0] callWithArguments:callbackArguments]];
            });
        }

        return timerID;
    };

    context[@"setInterval"] = ^() {
        JSPageScope *pageScope = JS::JSPageScopeTracer::CurrentPageScope();
        NSArray *arguments = [JSContext currentArguments];
        long timerID = -1;
        if (arguments.count > 0) {
            NSArray *callbackArguments = arguments.count > 2 ? [arguments subarrayWithRange:NSMakeRange(2, arguments.count - 2)] : @[];
            long interval = arguments.count > 1 ? [[arguments[1] toNumber] longValue] : 0;
            timerID = AtomGraphics::ScriptTimerController::CreateTimerID(interval, true, [pageScope, arguments, callbackArguments]() {
                JS::JSPageScopeTracer tracer(pageScope);
                [JSExecutionChecker checkFunctionExecutionResult:[arguments[0] callWithArguments:callbackArguments]];
            });
        }
        return timerID;
    };

    void (^clearTimerFunc)(JSValue *) = ^(JSValue *value) {
        if ([value isNumber]) {
            long timerID = [value toInt32];
            AtomGraphics::ScriptTimerController::RemoveTimerID(timerID);
        }
    };

    context[@"clearTimeout"] = clearTimerFunc;
    context[@"clearInterval"] = clearTimerFunc;


    context[@"requestAnimationFrame"] = ^(JSValue *callback) {
        JSPageScope *pageScope = JS::JSPageScopeTracer::CurrentPageScope();
        long callbackID = -1;
        if (callback) {
            callbackID = ScriptAnimationFrameController::SharedInstance()->addCallback(RequestAnimationCallback::Create([pageScope, callback]() {
                JS::JSPageScopeTracer tracer(pageScope);
                [JSExecutionChecker checkFunctionExecutionResult:[callback callWithArguments:nil]];
            }));
            GraphicsContentFlushController::SharedInstance()->scheduleLayerFlush();
        }
        return callbackID;
    };

    context[@"cancelAnimationFrame"] = ^(long callbackID) {
        ScriptAnimationFrameController::SharedInstance()->cancelCallback(callbackID);
    };

    context[@"devicePixelRatio"] = @(Device::DeviceScaleFactor());

    context[@"document"] = [JSDocument new];
}

@end