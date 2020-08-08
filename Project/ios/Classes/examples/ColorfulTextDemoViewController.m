//
// Created by neo on 2019-09-16.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "ColorfulTextDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation ColorfulTextDemoViewController {

}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
}


- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent colorfulTextJSContent]];
}

@end