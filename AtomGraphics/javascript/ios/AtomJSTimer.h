//
// Created by neo on 2018/4/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GraphicsPageContext.h"


@interface AtomJSTimer : NSObject

+ (void)initJSTimerAPIWithJSContext:(JSContext *)context andPageContext:(AtomGraphics::GraphicsPageContext *)pageContext;

@end