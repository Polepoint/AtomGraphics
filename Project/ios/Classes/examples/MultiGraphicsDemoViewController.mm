//
// Created by neo on 2019-09-05.
// Copyright (c) 2019 neo. All rights reserved.
//

#import <AtomGraphics/AtomGraphicsView.h>
#import "MultiGraphicsDemoViewController.h"
#import "GlobalResourceContent.h"

@interface MultiGraphicsDemoViewController ()

@property(nonatomic, strong) UIScrollView *scrollView;
@property(nonatomic, strong) AtomGraphicsView *graphicsView0;
@property(nonatomic, strong) AtomGraphicsView *graphicsView1;
@property(nonatomic, strong) AtomGraphicsView *graphicsView2;

@end


@implementation MultiGraphicsDemoViewController {
    BOOL _graphicsLoaded;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];

    self.scrollView.frame = self.view.bounds;
    [self.view addSubview:self.scrollView];

    CGFloat frameWidth = self.view.frame.size.width;
    [self graphicsView0].frame = CGRectMake(0, 0, frameWidth, 400);
    [self.scrollView addSubview:[self graphicsView0]];

    [self graphicsView1].frame = CGRectMake(0, 500, frameWidth, 400);
    [self.scrollView addSubview:[self graphicsView1]];

    [self graphicsView2].frame = CGRectMake(0, 1000, frameWidth, 400);
    [self.scrollView addSubview:[self graphicsView2]];
}


- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    if (!_graphicsLoaded) {
        _graphicsLoaded = YES;
        [self loadGraphicsView:[self graphicsView0] withEntryScript:[GlobalResourceContent clockJSContent]];
        [self loadGraphicsView:[self graphicsView1] withEntryScript:[GlobalResourceContent loadingJSContent]];
        [self loadGraphicsView:[self graphicsView2] withEntryScript:[GlobalResourceContent hanabiJSContent]];
    }
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    if (@available(iOS 11.0, *)) {
        self.scrollView.frame = UIEdgeInsetsInsetRect(self.view.bounds, self.view.safeAreaInsets);
        self.scrollView.contentSize = CGSizeMake(self.scrollView.bounds.size.width, 1400);
    } else {
        CGFloat topPosition = CGRectGetMaxY(self.navigationController.navigationBar.frame);
        self.scrollView.frame = CGRectMake(0, topPosition, self.view.frame.size.width, self.view.frame.size.height - topPosition);
        self.scrollView.contentSize = CGSizeMake(self.scrollView.bounds.size.width, 1400);
    }
}


- (void)loadGraphicsView:(AtomGraphicsView *)graphicsView withEntryScript:(NSString *)entryFileContent {

    [graphicsView evaluateScript:entryFileContent];
}

- (void)dealloc {

    if (_graphicsView0) {
        [_graphicsView0 callJSHandler:@"dispose"];
    }

    if (_graphicsView1) {
        [_graphicsView1 callJSHandler:@"dispose"];
    }

    if (_graphicsView2) {
        [_graphicsView2 callJSHandler:@"dispose"];
    }
}


#pragma mark - Getters & Setters


- (UIScrollView *)scrollView {
    if (!_scrollView) {
        _scrollView = [[UIScrollView alloc] init];
        _scrollView.backgroundColor = [UIColor whiteColor];
        [self.view addSubview:_scrollView];
    }

    return _scrollView;
}


- (AtomGraphicsView *)graphicsView0 {
    if (!_graphicsView0) {
        _graphicsView0 = [[AtomGraphicsView alloc] init];
        _graphicsView0.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    }

    return _graphicsView0;
}


- (AtomGraphicsView *)graphicsView1 {
    if (!_graphicsView1) {
        _graphicsView1 = [[AtomGraphicsView alloc] init];
        _graphicsView1.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    }

    return _graphicsView1;
}

- (AtomGraphicsView *)graphicsView2 {
    if (!_graphicsView2) {
        _graphicsView2 = [[AtomGraphicsView alloc] init];
        _graphicsView2.autoresizingMask = UIViewAutoresizingFlexibleWidth;
    }

    return _graphicsView2;
}


@end