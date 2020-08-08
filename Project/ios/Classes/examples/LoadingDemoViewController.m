//
// Created by neo on 2019-09-14.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "LoadingDemoViewController.h"
#import "GlobalResourceContent.h"

@implementation LoadingDemoViewController

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent loadingJSContent]];
}

@end
