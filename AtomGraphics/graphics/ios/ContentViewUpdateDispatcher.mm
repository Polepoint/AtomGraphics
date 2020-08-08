//
// Created by neo on 2018/11/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "ContentViewUpdateDispatcher.h"
#import "GraphicsContentView.h"


using namespace AtomGraphics;

@interface ContentViewUpdateDispatcher ()

@property(nonatomic, strong) NSMutableDictionary<NSNumber *, GraphicsContentView *> *contentViews;

@end

@implementation ContentViewUpdateDispatcher {

}

+ (instancetype)singleton {

    static ContentViewUpdateDispatcher *dispatcher;
    if (!dispatcher) {
        dispatcher = [ContentViewUpdateDispatcher new];
    }

    return dispatcher;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _contentViews = [NSMutableDictionary<NSNumber *, GraphicsContentView *> dictionary];
    }

    return self;
}

- (void)registerContentView:(GraphicsContentView *)view withPageID:(long)pageID {
    @synchronized (self) {
        _contentViews[@(pageID)] = view;
    }
}

- (void)unregisterContentView:(long)pageID {
    @synchronized (self) {
        [_contentViews removeObjectForKey:@(pageID)];
    }
}

- (void)dispatchTransaction:(TransactionBunch *)bunch {
    NSDictionary *contentViewsCopy;
    @synchronized (self) {
        contentViewsCopy = [_contentViews copy];
    }

    NSArray *pageIDs = contentViewsCopy.allKeys;
    for (NSNumber *pageID in pageIDs) {
        Transaction *transaction = bunch->getTransaction([pageID longValue]);
        if (transaction) {
            [contentViewsCopy[pageID] applyTransaction:transaction];
        }
    }
}


@end