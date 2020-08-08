//
// Created by neo on 2018/11/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TransactionBunch.h"

@class GraphicsContentView;


@interface ContentViewUpdateDispatcher : NSObject

+ (instancetype)singleton;

- (void)registerContentView:(GraphicsContentView *)view withPageID:(long)pageID;

- (void)unregisterContentView:(long)pageID;

- (void)dispatchTransaction:(AtomGraphics::TransactionBunch *)bunch;

@end