//
// Created by neo on 2020/2/21.
// Copyright (c) 2020 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface MonitorLines : UIView

@property(nonatomic, assign) NSInteger maxFPS;

- (void)recordFPS:(int)FPS usedMemory:(vm_size_t)usedMemorySize usedCPU:(float)usedCPU;

@end