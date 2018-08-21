//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "ChartGroupView.h"
#import "AtomJavaScriptCore.h"
#import "AtomGraphicsView.h"

@interface ChartGroupView ()

@end


@implementation ChartGroupView {
    AtomGraphicsView *_chart0;
    AtomGraphicsView *_chart1;
    AtomGraphicsView *_chart2;
    AtomGraphicsView *_chart3;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self initSubviews];
    }

    return self;
}

- (void)initSubviews {
    CGFloat halfWidth = self.bounds.size.width / 2;
    CGFloat halfHeight = self.bounds.size.height / 2;
    _chart0 = [[AtomGraphicsView alloc] initWithFrame:CGRectMake(0, 0, halfWidth, halfHeight)];
    _chart1 = [[AtomGraphicsView alloc] initWithFrame:CGRectMake(halfWidth, 0, halfWidth, halfHeight)];
    _chart2 = [[AtomGraphicsView alloc] initWithFrame:CGRectMake(0, halfHeight, halfWidth, halfHeight)];
    _chart3 = [[AtomGraphicsView alloc] initWithFrame:CGRectMake(halfWidth, halfHeight, halfWidth, halfHeight)];
    [self addSubview:_chart0];
    [self addSubview:_chart1];
    [self addSubview:_chart2];
    [self addSubview:_chart3];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    CGFloat halfWidth = self.bounds.size.width / 2;
    CGFloat halfHeight = self.bounds.size.height / 2;
    _chart0.frame = CGRectMake(0, 0, halfWidth, halfHeight);
    _chart1.frame = CGRectMake(halfWidth, 0, halfWidth, halfHeight);
    _chart2.frame = CGRectMake(0, halfHeight, halfWidth, halfHeight);
    _chart3.frame = CGRectMake(halfWidth, halfHeight, halfWidth, halfHeight);
}


- (void)didMoveToWindow {
    [super didMoveToWindow];
    [[AtomJavaScriptCore javaScriptCoreForContext:[AtomGraphicsView PageContext]] runScriptFile:[[NSBundle mainBundle] pathForResource:@"main" ofType:@"js"]];
}

- (void)reloadGraphics {
    [[AtomJavaScriptCore javaScriptCoreForContext:[AtomGraphicsView PageContext]] runScript:@"redraw()"];
}

- (void)reloadGraphicsAtIndex:(NSUInteger)index {
    [[AtomJavaScriptCore javaScriptCoreForContext:[AtomGraphicsView PageContext]] runScript:[NSString stringWithFormat:@"redraw(%u)", index]];
}

@end