//
// Created by neo on 2019-09-16.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "DemoViewController.h"


@implementation DemoViewController {
    BOOL _graphicsLoaded;

}


- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    if (!_graphicsLoaded) {
        [self loadJSContent];
        _graphicsLoaded = YES;
    }
}


- (void)loadJSContent {

}


- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    if (@available(iOS 11.0, *)) {
        self.graphicsView.frame = UIEdgeInsetsInsetRect(self.view.bounds, self.view.safeAreaInsets);
    } else {
        CGFloat topPosition = CGRectGetMaxY(self.navigationController.navigationBar.frame);
        self.graphicsView.frame = CGRectMake(0, topPosition, self.view.frame.size.width, self.view.frame.size.height - topPosition);
    }
}


- (void)dealloc {
    if (_graphicsView) {
        [_graphicsView callJSHandler:@"dispose"];
    }
}

#pragma mark - Getters & Setters

- (AtomGraphicsView *)graphicsView {
    if (!_graphicsView) {
        _graphicsView = [[AtomGraphicsView alloc] init];
        [self.view addSubview:_graphicsView];
    }
    return _graphicsView;
}


@end