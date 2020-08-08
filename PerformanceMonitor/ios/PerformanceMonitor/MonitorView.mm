//
// Created by neo on 2020/2/19.
// Copyright (c) 2020 neo. All rights reserved.
//

#import <mach/task.h>
#import <mach/mach_init.h>
#import <mach/thread_act.h>
#import <mach/vm_map.h>
#import "MonitorView.h"
#import "MonitorLines.h"

static vm_size_t profileGetMemoryUsage() {
    vm_size_t memoryUsageInByte = 0;
    task_vm_info_data_t vmInfo;
    mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
    kern_return_t kernelReturn = task_info(mach_task_self(), TASK_VM_INFO, (task_info_t) &vmInfo, &count);
    if (kernelReturn == KERN_SUCCESS) {
        memoryUsageInByte = (vm_size_t) vmInfo.phys_footprint;
    }

    return memoryUsageInByte;
}

static float profileCPUUsage() {

    kern_return_t kr;
    thread_array_t thread_list;
    mach_msg_type_number_t thread_count;
    thread_info_data_t thinfo;
    mach_msg_type_number_t thread_info_count;
    thread_basic_info_t basic_info_th;

    kr = task_threads(mach_task_self(), &thread_list, &thread_count);
    if (kr != KERN_SUCCESS)
        return -1;

    float tot_cpu = 0;

    for (int j = 0; j < thread_count; j++) {
        thread_info_count = THREAD_INFO_MAX;
        kr = thread_info(thread_list[j], THREAD_BASIC_INFO, (thread_info_t) thinfo, &thread_info_count);
        if (kr != KERN_SUCCESS)
            return -1;

        basic_info_th = (thread_basic_info_t) thinfo;
        if (!(basic_info_th->flags & TH_FLAGS_IDLE)) {
            tot_cpu += basic_info_th->cpu_usage / (float) TH_USAGE_SCALE;
        }

    }

    kr = vm_deallocate(mach_task_self(), (vm_offset_t) thread_list, thread_count * sizeof(thread_t));
    assert(kr == KERN_SUCCESS);

    if (tot_cpu < 0) {
        tot_cpu = 0.f;
    }

    return tot_cpu;

}


static unsigned long long sDeviceMemory;
static NSInteger sMaxRefreshRate;

@interface MonitorView ()

@property(nonatomic, strong) UILabel *cpuLabel;
@property(nonatomic, strong) UILabel *memoryLabel;
@property(nonatomic, strong) UILabel *fpsLabel;
@property(nonatomic, strong) MonitorLines *monitorLines;

@end

@implementation MonitorView {
    NSTimeInterval _prevTimeInterval;
    int _frameCount;
    BOOL _linesShown;
    CGSize _shrinkSize;
    CGSize _expandSize;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = UIColor.grayColor;
        self.layer.borderWidth = 2;
        self.layer.borderColor = [UIColor lightGrayColor].CGColor;
        self.clipsToBounds = YES;
        UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTap)];
        tapGestureRecognizer.numberOfTapsRequired = 2;
        [self addGestureRecognizer:tapGestureRecognizer];

        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            sDeviceMemory = [NSProcessInfo processInfo].physicalMemory;
            if (@available(iOS 10.3, *)) {
                sMaxRefreshRate = [UIScreen mainScreen].maximumFramesPerSecond;
            } else {
                sMaxRefreshRate = 60;
            }
        });
    }

    return self;
}


- (void)onFrameStep:(NSTimeInterval)timeInterval {
    _frameCount++;
    if (timeInterval - _prevTimeInterval >= 1) {
        vm_size_t usedMem = profileGetMemoryUsage() >> 20;
        vm_size_t totalMem = (sDeviceMemory) >> 20;
        float cpuUsage = profileCPUUsage();
        self.memoryLabel.text = [NSString stringWithFormat:@"MEM: %ld / %ld MB (%.2lf%%)", usedMem, totalMem, 100.0 * usedMem / totalMem];
        self.fpsLabel.text = [NSString stringWithFormat:@"FPS: %d", _frameCount];
        self.cpuLabel.text = [NSString stringWithFormat:@"CPU: %.2f%%", cpuUsage * 100];
        [self.monitorLines recordFPS:_frameCount usedMemory:usedMem usedCPU:cpuUsage];
        if (_linesShown) {
            [self.monitorLines setNeedsLayout];
        }

        _prevTimeInterval = timeInterval;
        _frameCount = 0;
    }
}

- (void)onTap {
    _linesShown = !_linesShown;

    if (_linesShown) {
        _shrinkSize = self.frame.size;
        if (CGSizeEqualToSize(_expandSize, CGSizeZero)) {
            CGSize superviewSize;
            if (@available(iOS 11, *)) {
                superviewSize = UIEdgeInsetsInsetRect(self.superview.bounds, self.superview.safeAreaInsets).size;
            } else {
                superviewSize = self.superview.frame.size;
            }

            _expandSize = CGSizeMake(superviewSize.width - self.frame.origin.x - 10, superviewSize.height - self.frame.origin.y - 10);
        }
        self.frame = (CGRect) {
                self.frame.origin,
                _expandSize
        };
        self.monitorLines.hidden = NO;
    } else {
        _expandSize = self.frame.size;
        self.frame = (CGRect) {
                self.frame.origin,
                _shrinkSize
        };
        self.monitorLines.hidden = YES;
    }
}

- (UILabel *)fpsLabel {
    if (!_fpsLabel) {
        _fpsLabel = [[UILabel alloc] initWithFrame:CGRectMake(5, 0, self.bounds.size.width - 5, 30)];
        _fpsLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        _fpsLabel.textColor = [UIColor yellowColor];
        [self addSubview:_fpsLabel];
    }

    return _fpsLabel;
}

- (UILabel *)memoryLabel {
    if (!_memoryLabel) {
        _memoryLabel = [[UILabel alloc] initWithFrame:CGRectMake(5, 30, self.bounds.size.width - 5, 30)];
        _memoryLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        _memoryLabel.textColor = [UIColor purpleColor];
        [self addSubview:_memoryLabel];
    }

    return _memoryLabel;
}

- (UILabel *)cpuLabel {
    if (!_cpuLabel) {
        _cpuLabel = [[UILabel alloc] initWithFrame:CGRectMake(5, 60, self.bounds.size.width - 5, 30)];
        _cpuLabel.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        _cpuLabel.textColor = [UIColor cyanColor];
        [self addSubview:_cpuLabel];
    }

    return _cpuLabel;
}

- (MonitorLines *)monitorLines {
    if (!_monitorLines) {
        _monitorLines = [[MonitorLines alloc] initWithFrame:CGRectMake(0, 90, self.frame.size.width, self.frame.size.height - 90)];
        _monitorLines.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
        _monitorLines.hidden = !_linesShown;
        _monitorLines.maxFPS = sMaxRefreshRate;
        [self addSubview:_monitorLines];
    }
    return _monitorLines;
}


@end