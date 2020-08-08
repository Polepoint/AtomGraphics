//
// Created by neo on 2019-09-20.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "GalaxyDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation GalaxyDemoViewController {

}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
}


- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent galaxyJSContent]];
}


@end