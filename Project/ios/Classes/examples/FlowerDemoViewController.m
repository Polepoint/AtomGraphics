//
// Created by neo on 2019-09-14.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "FlowerDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation FlowerDemoViewController

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent flowerJSContent]];
}


@end
