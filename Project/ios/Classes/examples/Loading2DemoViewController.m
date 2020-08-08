//
// Created by neo on 2019-09-14.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "Loading2DemoViewController.h"
#import "GlobalResourceContent.h"

@implementation Loading2DemoViewController

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent loading2JSContent]];
}

@end
