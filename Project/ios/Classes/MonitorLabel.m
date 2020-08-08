//
// Created by neo on 2018/4/6.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <mach/task_info.h>
#import <mach/mach_init.h>
#import <mach/task.h>
#import "MonitorLabel.h"

@interface MonitorLabel ()

@property(nonatomic, strong) CADisplayLink *displayLink;
@property(nonatomic, strong) UIView *label;

@end

@implementation MonitorLabel {
    NSTimeInterval _prevTimeInterval;
    int _frameCount;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(record)];
        [_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
        _prevTimeInterval = [[NSDate date] timeIntervalSince1970];
        self.textAlignment = NSTextAlignmentLeft;
        self.font = [UIFont systemFontOfSize:14];
        self.textColor = [UIColor blackColor];
        self.numberOfLines = 0;
        self.backgroundColor = [[UIColor lightGrayColor] colorWithAlphaComponent:0.6];
        self.layer.borderColor = [UIColor lightGrayColor].CGColor;
        self.layer.borderWidth = 1;
    }

    return self;
}

- (nullable UIView *)hitTest:(CGPoint)point withEvent:(nullable UIEvent *)event {
    return nil;
}


- (int64_t)memoryUsage {
    int64_t memoryUsageInByte = 0;
    task_vm_info_data_t vmInfo;
    mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
    kern_return_t kernelReturn = task_info(mach_task_self(), TASK_VM_INFO, (task_info_t) &vmInfo, &count);
    if (kernelReturn == KERN_SUCCESS) {
        memoryUsageInByte = (int64_t) vmInfo.phys_footprint;
    }

    return memoryUsageInByte / 1024 / 1024;
}

- (void)record {
    _frameCount++;
    NSTimeInterval currentTimeInterval = [[NSDate date] timeIntervalSince1970];
    if (currentTimeInterval - _prevTimeInterval + 0.016 > 1) {
        _prevTimeInterval = currentTimeInterval;
        self.text = [NSString stringWithFormat:@" FPS: %d \n Mem: %lld MB", _frameCount, [self memoryUsage]];
        _frameCount = 0;
    }
}


@end