//
// Created by neo on 2020/2/18.
// Copyright (c) 2020 neo. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface PerformanceMonitor : NSObject

@property(nonatomic, readonly, assign) BOOL shown;

- (void)show;

- (void)hide;

@end