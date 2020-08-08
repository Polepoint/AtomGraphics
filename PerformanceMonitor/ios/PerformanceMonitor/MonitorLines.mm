//
// Created by neo on 2020/2/21.
// Copyright (c) 2020 neo. All rights reserved.
//

#import "MonitorLines.h"


struct LinePoint {
    int fps;
    float cpu_usage;
    vm_size_t memory_size;
    LinePoint *next;
    LinePoint *pre;
};

static const NSUInteger sPointCount = 60;

@interface MonitorLines ()

@property(nonatomic, strong) CAShapeLayer *fpsLayer;
@property(nonatomic, strong) CAShapeLayer *memoryLayer;
@property(nonatomic, strong) CAShapeLayer *cpuLayer;

@end

@implementation MonitorLines {
    LinePoint *_pointHeader;
    vm_size_t _maxMemory;
    CGPoint _axisOrigin;
    CGSize _axisSize;
    CGFloat _xAxisStep;
    CGFloat _fpsYAxisStep;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self initPoints];
    }

    return self;
}

- (void)dealloc {
    LinePoint *point = _pointHeader->next;
    while (point != _pointHeader) {
        LinePoint *next = point->next;
        delete point;
        point = next;
    }

    delete _pointHeader;
}

- (void)initPoints {
    LinePoint *tail = nullptr;
    for (int i = 0; i < sPointCount; i++) {
        LinePoint *point = new LinePoint();
        if (!_pointHeader) {
            _pointHeader = point;
            tail = _pointHeader;
        }

        tail->next = point;
        point->pre = tail;
        tail = point;
    }

    if (tail) {
        tail->next = _pointHeader;
        _pointHeader->pre = tail;
    }
}


- (void)recordFPS:(int)fps usedMemory:(vm_size_t)usedMemorySize usedCPU:(float)cpuUsage {

    _maxMemory = MAX(_maxMemory, usedMemorySize);
    if (_maxMemory > 0 && _maxMemory == _pointHeader->pre->memory_size) {
        _maxMemory = [self peakMemory];
    }

    _pointHeader->fps = fps;
    _pointHeader->memory_size = usedMemorySize;
    _pointHeader->cpu_usage = cpuUsage;
    _pointHeader = _pointHeader->next;
}

- (vm_size_t)peakMemory {
    vm_size_t maxMemory = _pointHeader->memory_size;

    LinePoint *point = _pointHeader->next;
    while (point != _pointHeader) {
        maxMemory = MAX(maxMemory, point->memory_size);
        point = point->next;
    }

    return maxMemory;
}

- (void)layoutSubviews {
    [super layoutSubviews];

    _axisOrigin = CGPointMake(5, CGRectGetMaxY(self.bounds) - 5);
    _axisSize = CGSizeMake(self.bounds.size.width - 10, self.bounds.size.height - 10);
    _xAxisStep = 1.0 * _axisSize.width / sPointCount;
    _fpsYAxisStep = 1.0 * _axisSize.height / _maxFPS;

    self.fpsLayer.frame = self.layer.bounds;
    self.memoryLayer.frame = self.layer.bounds;
    self.cpuLayer.frame = self.layer.bounds;
    [self drawLines];
}

- (void)drawLines {
    UIBezierPath *fpsShapePath = [UIBezierPath bezierPath];
    UIBezierPath *memoryShapePath = [UIBezierPath bezierPath];
    UIBezierPath *cpuShapePath = [UIBezierPath bezierPath];


    NSUInteger pointIndex = 1;
    LinePoint *point = _pointHeader->pre;

    [fpsShapePath moveToPoint:[self fpsPointLocationForLinePoint:point atIndex:0]];
    [memoryShapePath moveToPoint:[self memoryPointLocationForLinePoint:point atIndex:0]];
    [cpuShapePath moveToPoint:[self cpuPointLocationForLinePoint:point atIndex:0]];

    point = _pointHeader->pre;

    while (point != _pointHeader) {
        [fpsShapePath addLineToPoint:[self fpsPointLocationForLinePoint:point atIndex:pointIndex]];
        [memoryShapePath addLineToPoint:[self memoryPointLocationForLinePoint:point atIndex:pointIndex]];
        [cpuShapePath addLineToPoint:[self cpuPointLocationForLinePoint:point atIndex:pointIndex]];

        pointIndex++;
        point = point->pre;
    }

    self.fpsLayer.path = fpsShapePath.CGPath;
    self.memoryLayer.path = memoryShapePath.CGPath;
    self.cpuLayer.path = cpuShapePath.CGPath;
}

- (CGPoint)fpsPointLocationForLinePoint:(LinePoint *)point atIndex:(NSUInteger)index {
    CGPoint res = CGPointMake(_axisOrigin.x + index * _xAxisStep, _axisOrigin.y - point->fps * _fpsYAxisStep);
    return res;
}

- (CGPoint)memoryPointLocationForLinePoint:(LinePoint *)point atIndex:(NSUInteger)index {
    return CGPointMake(_axisOrigin.x + index * _xAxisStep, _axisOrigin.y - _axisSize.height * point->memory_size / _maxMemory);
}


- (CGPoint)cpuPointLocationForLinePoint:(LinePoint *)point atIndex:(NSUInteger)index {
    return CGPointMake(_axisOrigin.x + index * _xAxisStep, _axisOrigin.y - _axisSize.height * point->cpu_usage);
}


- (CAShapeLayer *)fpsLayer {
    if (!_fpsLayer) {
        _fpsLayer = [CAShapeLayer layer];
        _fpsLayer.lineWidth = 3;
        _fpsLayer.strokeColor = [[UIColor yellowColor] colorWithAlphaComponent:0.5].CGColor;
        _fpsLayer.fillColor = [UIColor clearColor].CGColor;
        [self.layer addSublayer:_fpsLayer];
    }

    return _fpsLayer;
}

- (CAShapeLayer *)memoryLayer {
    if (!_memoryLayer) {
        _memoryLayer = [CAShapeLayer layer];
        _memoryLayer.lineWidth = 3;
        _memoryLayer.strokeColor = [[UIColor purpleColor] colorWithAlphaComponent:0.5].CGColor;
        _memoryLayer.fillColor = [UIColor clearColor].CGColor;
        [self.layer addSublayer:_memoryLayer];
    }

    return _memoryLayer;
}

- (CAShapeLayer *)cpuLayer {
    if (!_cpuLayer) {
        _cpuLayer = [CAShapeLayer layer];
        _cpuLayer.lineWidth = 3;
        _cpuLayer.strokeColor = [[UIColor cyanColor] colorWithAlphaComponent:0.5].CGColor;
        _cpuLayer.fillColor = [UIColor clearColor].CGColor;
        [self.layer addSublayer:_cpuLayer];
    }

    return _cpuLayer;
}


@end