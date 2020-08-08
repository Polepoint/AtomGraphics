//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Transaction.h"

@class CompositingView;


@interface CompositingManager : NSObject

- (CompositingView *)useViewWithID:(long)viewID;

- (void)removeViewWithID:(long)viewID;

- (CompositingView *)createViewWithID:(long)viewID;

- (NSArray<NSNumber *> *)subviewIDsOfViewWithID:(long)viewID fromTransaction:(AtomGraphics::Transaction *)transaction;

- (NSArray *)viewList;

- (long)idSum;

@end