//
// Created by neo on 2018/4/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AtomCanvasNode.h"

typedef void (^AtomJSCoreGestureInitCallback)();

using namespace AtomGraphics;

typedef enum {
    AtomEventTypeUnknown = -1,
    AtomEventTypeStart = 0,
    AtomEventTypeMove,
    AtomEventTypeEnd
} AtomEventType;

@interface AtomJavaScriptCore : NSObject


+ (AtomJavaScriptCore *)javaScriptCoreForContext:(GraphicsPageContext *)pageContext;

- (void)runScriptFile:(NSString *)path;

- (void)runScript:(NSString *)script;

@end