//
// Created by neo on 2018/4/26.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "JSPageScope.h"
#import "GraphicsJavaScriptCore.h"
#import "GraphicsThread.h"
#import "JSPageScopeTracer.h"
#import "JSExecutionChecker.h"
#import "EventSender.h"

@interface JSPageScope ()

@property(nonatomic, strong) JSValue *pageScopeEvaluatorFunc;
@property(nonatomic, strong) JSValue *pageScopeRegisterNativeHandler;
@property(nonatomic, strong) JSValue *pageScopeCallJSHandler;

@end

@implementation JSPageScope {

}

+ (JSPageScope *)createPageJSScopeWithPageID:(long)pageID {
    NSBundle *bundle = [NSBundle bundleForClass:self.class];
    NSString *filePath = [bundle pathForResource:@"pageScope" ofType:@"js"];
    if (filePath) {
        JSPageScope *scope = [JSPageScope new];
        scope->_pageID = pageID;
        AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
            AtomGraphics::JS::JSPageScopeTracer tracer(scope);
            NSString *pageScopeJSContent = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:nil];
            JSContext *context = [[GraphicsJavaScriptCore sharedJavaScriptCore] context];
            JSValue *scopeCreatorFunction = [context evaluateScript:pageScopeJSContent];
            JSValue *funcObj = [scopeCreatorFunction callWithArguments:@[@(pageID)]];
            scope.pageScopeEvaluatorFunc = [funcObj valueForProperty:@"evaluate"];
            scope.pageScopeCallJSHandler = [funcObj valueForProperty:@"callJSHandler"];
            scope.pageScopeRegisterNativeHandler = [funcObj valueForProperty:@"registerNativeHandler"];
        });

        return scope;
    }

    return nil;
}

- (void)evaluateScript:(NSString *)script {
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        AtomGraphics::JS::JSPageScopeTracer tracer(self);
        [JSExecutionChecker checkFunctionExecutionResult:[self.pageScopeEvaluatorFunc callWithArguments:@[script]]];
    });
}

- (void)callJSHandler:(NSString *)handlerName parameters:(id)params {
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        AtomGraphics::JS::JSPageScopeTracer tracer(self);
        if (params) {
            [JSExecutionChecker checkFunctionExecutionResult:[self.pageScopeCallJSHandler callWithArguments:@[handlerName, params]]];
        } else {
            [JSExecutionChecker checkFunctionExecutionResult:[self.pageScopeCallJSHandler callWithArguments:@[handlerName]]];
        }
    });
}

- (void)registerNativeHandler:(NSString *)handlerName handlerCallback:(ScopeHandler)callback {
    ScopeHandler syncMainThreadBlock = ^(id data) {
        dispatch_async(dispatch_get_main_queue(), ^() {
            callback(data);
        });
    };
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        AtomGraphics::JS::JSPageScopeTracer tracer(self);
        [JSExecutionChecker checkFunctionExecutionResult:[self.pageScopeRegisterNativeHandler callWithArguments:@[handlerName, syncMainThreadBlock]]];
    });
}

- (void)destroy {
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        AtomGraphics::JS::JSPageScopeTracer tracer(self);
        AtomGraphics::EventSender::sender()
                ->dispatchEventImmediately(
                        AtomGraphics::EventTypePageScopeWillBeDestroyed,
                        nullptr);
    });
}

@end