//
// Created by neo on 2019-09-16.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "ClockDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation ClockDemoViewController {

}

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent clockJSContent]];
}


@end