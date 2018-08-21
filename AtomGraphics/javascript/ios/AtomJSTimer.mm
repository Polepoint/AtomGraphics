//
// Created by neo on 2018/4/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomJSTimer.h"
#import "Timer.h"
#import "ScriptTimer.h"
#import "ScriptAnimationFrameController.h"
#import "GraphicsContentFlushController.h"

using namespace AtomGraphics;

@implementation AtomJSTimer


+ (void)initJSTimerAPIWithJSContext:(JSContext *)context andPageContext:(AtomGraphics::GraphicsPageContext *)pageContext {
    NSString *jsCoreFilePath = [[NSBundle mainBundle] pathForResource:@"JSTimers" ofType:@"js"];
    [context evaluateScript:[NSString stringWithContentsOfFile:jsCoreFilePath encoding:NSUTF8StringEncoding error:nil]];
    JSValue *window = context[@"window"];

    [window setValue:^(JSValue *callback, float time) {
        NSArray *arguments = [JSContext currentArguments];
        long timerID = -1;
        if (arguments.count > 1) {
            NSArray *callbackArguments = arguments.count > 2 ? [arguments subarrayWithRange:NSMakeRange(2, arguments.count - 2)] : @[];
            timerID = AtomGraphics::ScriptTimer::CreateTimerID([[arguments[1] toNumber] longValue], false, [arguments, callbackArguments]() {
                [arguments[0] callWithArguments:callbackArguments];
            });
        }

        return timerID;
    }    forProperty:@"setTimeout"];

    [window setValue:^(JSValue *callback, float time) {
        NSArray *arguments = [JSContext currentArguments];
        long timerID = -1;
        if (arguments.count > 1) {
            NSArray *callbackArguments = arguments.count > 2 ? [arguments subarrayWithRange:NSMakeRange(2, arguments.count - 2)] : @[];
            timerID = AtomGraphics::ScriptTimer::CreateTimerID([[arguments[1] toNumber] longValue], true, [arguments, callbackArguments]() {
                [arguments[0] callWithArguments:callbackArguments];
            });
        }
        return timerID;
    }    forProperty:@"setInterval"];

    [window setValue:^(JSValue *value) {
        if ([value isNumber]) {
            long timerID = [value toInt32];
            AtomGraphics::ScriptTimer::RemoveTimerID(timerID);
        }
    }    forProperty:@"clearTimeout"];

    [window setValue:^(JSValue *value) {
        if ([value isNumber]) {
            long timerID = [value toInt32];
            AtomGraphics::ScriptTimer::RemoveTimerID(timerID);
        }
    }    forProperty:@"clearInterval"];


    ScriptAnimationFrameController *animationFrameController = ScriptAnimationFrameController::ControllerForContext(pageContext);

    [window setValue:^(JSValue *callback) {
        long callbackID = -1;
        if (callback) {
            callbackID = animationFrameController->addCallback(new RequestAnimationCallback([callback]() {
                [callback callWithArguments:nil];
            }));
            pageContext->flushController()->scheduleLayerFlush();
        }
        return callbackID;
    }    forProperty:@"requestAnimationFrame"];

    [window setValue:^(long callbackID) {
        animationFrameController->cancelCallback(callbackID);
    }    forProperty:@"cancelAnimationFrame"];
}

@end