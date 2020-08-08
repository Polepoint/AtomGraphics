//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "CompositingManager.h"
#import "CompositingView.h"

using namespace AtomGraphics;

@interface CompositingManager ()

@property(nonatomic, strong) NSMutableDictionary<NSNumber *, CompositingView *> *viewTree;

@property(nonatomic, strong) NSMutableDictionary<NSNumber *, CompositingView *> *viewTreeToRemove;

@property(nonatomic, strong) NSTimer *pruneTimer;

@end

@implementation CompositingManager {
    long _idSummary;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _viewTree = [NSMutableDictionary dictionary];
        _viewTreeToRemove = [NSMutableDictionary dictionary];
    }
    
    return self;
}

- (CompositingView *)useViewWithID:(long)viewID {
    NSNumber *key = @(viewID);
    CompositingView *view = _viewTree[key];
    if (view) {
        return view;
    }
    
    view = _viewTreeToRemove[key];
    if (view) {
        _viewTree[key] = _viewTreeToRemove[key];
        [_viewTreeToRemove removeObjectForKey:key];
        return view;
    }
    
    return view;
}

- (void)removeViewWithID:(long)viewID {
    _viewTreeToRemove[@(viewID)] = _viewTree[@(viewID)];
    [_viewTree removeObjectForKey:@(viewID)];
    _idSummary -= viewID;
    [self schedulePruneTimer];
}

- (void)schedulePruneTimer {
    if (_pruneTimer) {
        [_pruneTimer invalidate];
    }
    _pruneTimer = [NSTimer scheduledTimerWithTimeInterval:5 target:self selector:@selector(pruneTimerFired) userInfo:nil repeats:NO];
}

- (void)pruneTimerFired {
    _pruneTimer = nil;
    [_viewTreeToRemove removeAllObjects];
}

- (CompositingView *)createViewWithID:(long)viewID {
    CompositingView *view = [CompositingView new];
    view.viewID = viewID;
    _viewTree[@(viewID)] = view;
    _idSummary += viewID;
    
    return view;
}

- (NSArray<NSNumber *> *)subviewIDsOfViewWithID:(long)viewID fromTransaction:(Transaction *)transaction {
    NSMutableArray *array = [NSMutableArray array];
    std::vector<TransactionProperties> propertyList = transaction->getPropertyList();
    for (TransactionProperties &properties : propertyList) {
        if (properties.m_superlayerID == viewID) {
            [array addObject:@(properties.m_layerID)];
        }
    }
    
    return array;
}


- (NSArray *)viewList {
    return _viewTree.allValues;
}

- (long)idSum {
    return _idSummary;
}


@end