//
// Created by neo on 2019-09-16.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "HeartDemoViewController.h"
#import "GlobalResourceContent.h"

@implementation HeartDemoViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
}


- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent heartJSContent]];
}

@end