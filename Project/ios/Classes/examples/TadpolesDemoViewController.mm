//
// Created by neo on 2019-09-05.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "TadpolesDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation TadpolesDemoViewController

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent tadpolesJSContent]];
}

@end