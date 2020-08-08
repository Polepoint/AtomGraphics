//
// Created by neo on 2018/4/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsJavaScriptCore.h"
#import "JSWindow.h"
#import "JSCanvas.h"
#import "GraphicsPageManager.h"
#import "CanvasNodeCG.h"
#import "GraphicsThread.h"
#import "GraphicsLayer.h"
#import "JSImageBitmap.h"
#import "GraphicsEnvironment.h"

using namespace AtomGraphics;

@implementation GraphicsJavaScriptCore

+ (void)initialize {
    [super initialize];
    [GraphicsEnvironment initializeEnvironment];
}

+ (GraphicsJavaScriptCore *)sharedJavaScriptCore {
    static GraphicsJavaScriptCore *javaScriptCore;
    if (!javaScriptCore) {
        javaScriptCore = [[GraphicsJavaScriptCore alloc] init];
    }

    return javaScriptCore;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        __weak typeof(self) wealSelf = self;
        AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
            [wealSelf initContext];
            [JSWindow setupContext:wealSelf.context];
        });
    }

    return self;
}


- (JSCanvas *)getCanvasNodeWithPageID:(long)pageID {

    JSCanvas *canvas;
    GraphicsPage *page = GraphicsPageManager::getPageByID(pageID);
    if (page) {
        auto node = page->rootLayer()->getRootNode();
        if (!node) {
            node.reset(new CanvasNodeCG);
            page->rootLayer()->setRootNode(node);
        }

        canvas = [JSCanvas wrapCanvasNode:node];
    }

    return canvas;
}

- (void)initContext {
    NSBundle *bundle = [NSBundle bundleForClass:self.class];
    NSString *jsCoreFilePath = [bundle pathForResource:@"core" ofType:@"js"];
    if (!jsCoreFilePath) {
        return;
    }

    NSString *jsCoreFileContent = [NSString stringWithContentsOfFile:jsCoreFilePath encoding:NSUTF8StringEncoding error:nil];
    _context = [JSContext new];
    static int contextIndex = 0;
    _context.name = [NSString stringWithFormat:@"ATG Global JSContext %d", contextIndex++];
    [_context evaluateScript:jsCoreFileContent];
    [self initContextGlobal];
#if DEBUG
    [_context setExceptionHandler:^(JSContext *context, JSValue *exception) {
        NSLog(@"AtomGraphics - JS Exception: %@", exception);
    }];
#endif
}

- (void)initContextGlobal {

    JSValue *AG = _context[@"AG"];

    __weak typeof(self) weakSelf = self;
    [AG  setValue:^(JSValue *idValue) {
        JSCanvas *anInterface = nil;
        if ([idValue isNumber]) {
            anInterface = [weakSelf getCanvasNodeWithPageID:[idValue toInt32]];
        }

        return anInterface;
    } forProperty:@"getPageRootNodeById"];

    [AG  setValue:^() {//创建一个没有绑定view的canvasNode，同时具备对应的backingStore，不参与绘制，但保存绘制的过程
        std::shared_ptr<Node> node(new CanvasNodeCG());
        JSCanvas *canvasJavaScriptInterface = [[JSCanvas alloc] initWithCanvasNode:node];
        return canvasJavaScriptInterface;
    } forProperty:@"createCanvasNode"];

    [AG  setValue:^(NSString *logStr) {
#if DEBUG
        NSLog(@"AtomGraphics - JavaScript log: %@", logStr);
#endif
    } forProperty:@"log"];

    [AG  setValue:^() {//创建Image 等同 new Image()
        JSImageBitmap *imageJavaScriptInterface = [[JSImageBitmap alloc] init];
        return imageJavaScriptInterface;
    } forProperty:@"createImage"];
}

- (void)runScriptFile:(NSURL *)sourceURL {
    if (sourceURL) {
        AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
            [self.context evaluateScript:[NSString stringWithContentsOfURL:sourceURL encoding:NSUTF8StringEncoding error:nil] withSourceURL:sourceURL];
        });
    }
}

- (void)runScript:(NSString *)script {
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        [self.context evaluateScript:script];
    });
}

- (void)runScript:(NSString *)script debugSource:(NSURL *)sourceUrl {
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        [self.context evaluateScript:script withSourceURL:sourceUrl];
    });
}

- (void)reset {
    __weak typeof(self) wealSelf = self;
    AtomGraphics::GraphicsThread::DispatchOnGraphicsQueue(^() {
        [wealSelf initContext];
        [JSWindow setupContext:wealSelf.context];
    });
}

@end

