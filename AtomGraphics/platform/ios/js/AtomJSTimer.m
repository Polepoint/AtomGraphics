//
// Created by neo on 2018/4/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomJSTimer.h"

static NSMutableDictionary<NSString *, AtomJSTimer *> *sharedTimers;


@interface AtomJSTimer ()

@property(nonatomic, strong) NSTimer *timer;

@property(nonatomic, copy) void (^invokeFunction)();

@end

@implementation AtomJSTimer {

}

+ (void)initJSTimers:(JSContext *)context {
    if (!sharedTimers) {
        sharedTimers = [NSMutableDictionary dictionary];
    }

    NSString *jsCoreFilePath = [[NSBundle mainBundle] pathForResource:@"JSTimers" ofType:@"js"];
    [context evaluateScript:[NSString stringWithContentsOfFile:jsCoreFilePath]];
    JSValue *window = context[@"window"];

    [window setValue:^(JSValue *callback, CGFloat time) {
        NSArray *arguments = [JSContext currentArguments];
        NSString *timerID;
        if (arguments.count > 1) {
            NSArray *callbackArguments = arguments.count > 2 ? [arguments subarrayWithRange:NSMakeRange(2, arguments.count - 2)] : @[];
            timerID = [AtomJSTimer createTimerMillionSeconds:[arguments[1] longLongValue] repeats:NO callback:arguments[0] arguments:callbackArguments];
        }

        return timerID;
    }    forProperty:@"setTimeout"];

    [window setValue:^(JSValue *callback, CGFloat time) {
        NSArray *arguments = [JSContext currentArguments];
        NSString *timerID;
        if (arguments.count > 1) {
            NSArray *callbackArguments = arguments.count > 2 ? [arguments subarrayWithRange:NSMakeRange(2, arguments.count - 2)] : @[];
            timerID = [AtomJSTimer createTimerMillionSeconds:[arguments[1] longLongValue] repeats:YES callback:arguments[0] arguments:callbackArguments];
        }

        return timerID;

    }    forProperty:@"setInterval"];

    [window setValue:^(NSString *timerID) {
        [AtomJSTimer clearTimer:timerID];
    }    forProperty:@"clearTimeout"];

    [window setValue:^(NSString *timerID) {
        [AtomJSTimer clearTimer:timerID];
    }    forProperty:@"clearInterval"];
}


+ (NSString *)createTimerMillionSeconds:(long long)millionSeconds repeats:(BOOL)yesOrNo callback:(JSValue *)callbackFunc arguments:(NSArray *)arguments {
    NSString *timerID = [[NSUUID UUID] UUIDString];
    AtomJSTimer *timer = [AtomJSTimer timerWithTimeInterval:millionSeconds / 1000.0 repeats:yesOrNo invokeFunction:^() {
        [callbackFunc callWithArguments:arguments];
    }];
    sharedTimers[timerID] = timer;
    return timerID;
}

+ (void)clearTimer:(NSString *)timerID {
    AtomJSTimer *timer = sharedTimers[timerID];
    if (timer) {
        [timer.timer invalidate];
        [sharedTimers removeObjectForKey:timerID];
    }
}

+ (AtomJSTimer *)timerWithTimeInterval:(NSTimeInterval)ti repeats:(BOOL)yesOrNo invokeFunction:(void (^)())function {
    AtomJSTimer *timer = [AtomJSTimer new];
    if (self) {
        timer.timer = [NSTimer timerWithTimeInterval:ti target:timer selector:@selector(invoke) userInfo:nil repeats:yesOrNo];
        timer.invokeFunction = function;
    }

    return timer;
}

- (void)invoke {
    _invokeFunction();
}


@end