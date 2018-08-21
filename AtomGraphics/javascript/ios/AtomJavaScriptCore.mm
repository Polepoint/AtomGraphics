//
// Created by neo on 2018/4/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomJavaScriptCore.h"
#import "AtomJSTimer.h"
#import "AtomJSZRender.h"
#import "AtomCanvasContext2D.h"
#import "AtomGCanvasNode.h"
#import "CanvasJavaScriptInterface.h"
#import "GraphicsPageManager.h"

@interface AtomJavaScriptCore ()

@property(nonatomic, copy) AtomJSCoreGestureInitCallback gestureInitCallback;
@property(nonatomic, strong) JSContext *jsContext;

@property(nonatomic, strong) JSValue *chart;

@property(nonatomic, strong) NSCache<NSNumber *, CanvasJavaScriptInterface *> *rootNodeCache;

@end

@implementation AtomJavaScriptCore

+ (AtomJavaScriptCore *)javaScriptCoreForContext:(GraphicsPageContext *)pageContext {
    static NSMutableDictionary<NSNumber *, AtomJavaScriptCore *> *jscMap = [NSMutableDictionary dictionary];
    NSNumber *key = @(reinterpret_cast<long>(pageContext));
    AtomJavaScriptCore *javaScriptCore = jscMap[key];
    if (!javaScriptCore) {
        javaScriptCore = [[AtomJavaScriptCore alloc] initWithPageContext:pageContext];
        jscMap[key] = javaScriptCore;
    }

    return javaScriptCore;
}

- (instancetype)initWithPageContext:(GraphicsPageContext *)pageContext {
    self = [super init];
    if (self) {
        NSString *jsCoreFilePath = [[NSBundle mainBundle] pathForResource:@"core" ofType:@"js"];
        if (jsCoreFilePath) {
            _jsContext = [[JSContext alloc] init];
            [_jsContext evaluateScript:[NSString stringWithContentsOfFile:jsCoreFilePath]];
            [self initGlobalContext];
            [AtomJSTimer initJSTimerAPIWithJSContext:_jsContext andPageContext:pageContext];
            [AtomJSZRender initJSZRender:_jsContext];
            _rootNodeCache = [NSCache new];
        }
    }

    return self;
}


- (CanvasJavaScriptInterface *)getCanvasNodeWithPageID:(long)pageID {

    CanvasJavaScriptInterface *anInterface = [_rootNodeCache objectForKey:@(pageID)];
    if (!anInterface) {
        GraphicsPage *page = GraphicsPageManager::getPageByID(pageID);
        if (page) {
            CanvasNode *node = dynamic_cast<CanvasNode *>(page->rootLayer()->getRootNode());
            anInterface = [[CanvasJavaScriptInterface alloc] initWithCanvasNode:node];
            [_rootNodeCache setObject:anInterface forKey:@(pageID)];
        }
    }

    return anInterface;
}

- (void)initGlobalContext {

    JSValue *AG = _jsContext[@"AG"];

    __weak typeof(self) weakSelf = self;
    [AG  setValue:^(JSValue *idValue) {
        CanvasJavaScriptInterface *anInterface = nil;
        if ([idValue isNumber]) {
            return [weakSelf getCanvasNodeWithPageID:[idValue toInt32]];
        }

        return anInterface;
    } forProperty:@"getPageRootNodeById"];

    [AG  setValue:^() {//创建一个没有绑定view的canvasNode，同时具备对应的backingStore，不参与绘制，但保存绘制的过程
        GCanvasNode *node = new GCanvasNode();
        CanvasJavaScriptInterface *canvasJavaScriptInterface = [[CanvasJavaScriptInterface alloc] initWithCanvasNode:node];
        return canvasJavaScriptInterface;
    } forProperty:@"createCanvasNode"];

    [AG  setValue:^() {
        NSString *optionsFilePath = [[NSBundle mainBundle] pathForResource:@"options" ofType:@"json"];
        NSError *error;
        NSString *jsonStr = [[NSString alloc] initWithContentsOfFile:optionsFilePath encoding:NSUTF8StringEncoding error:&error];

        NSData *jsonData = [jsonStr dataUsingEncoding:NSUTF8StringEncoding];
        NSError *err;
        NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                            options:NSJSONReadingMutableContainers
                                                              error:&err];
        if (err) {
            NSLog(@"failed to parse json :%@", err);
        }

        JSValue *value = [JSValue valueWithObject:dic inContext:[JSContext currentContext]];
        return value;
    } forProperty:@"getOptionsFromBundle"];

    [AG  setValue:^(JSValue *chart) {
        _chart = chart;
    } forProperty:@"registerChart"];

    [AG  setValue:^(JSValue *chart) {
        if (_gestureInitCallback) {
            _gestureInitCallback();
        }
    } forProperty:@"initGestures"];
}

- (void)runScriptFile:(NSString *)path {
    if (path) {
        AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
            [_jsContext evaluateScript:[NSString stringWithContentsOfFile:path]];
        });
    }
}

- (void)runScript:(NSString *)script {
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        [_jsContext evaluateScript:script];
    });
}

@end

