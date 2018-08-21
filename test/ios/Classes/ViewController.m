//
//  ViewController.m
//  AtomGraphics
//
//  Created by neo on 2017/11/15.
//  Copyright © 2017年 neo. All rights reserved.
//

#import "ViewController.h"
#import "FPSLabel.h"
#import "ChartGroupView.h"

@interface ViewController ()

@end

@implementation ViewController {
    ChartGroupView *_contentView;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    _contentView = [[ChartGroupView alloc] initWithFrame:self.view.bounds];
    _contentView.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:_contentView];
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:[[FPSLabel alloc] initWithFrame:CGRectMake(0, 0, 100, 40)]];
    UIButton *reloadButton = [self createButton:@"Reload" action:@selector(reload) frame:CGRectMake(0, 0, 80, 40)];
    UIButton *reloadButton0 = [self createButton:@"Reload0" action:@selector(reload0) frame:CGRectMake(90, 0, 80, 40)];
    UIButton *reloadButton1 = [self createButton:@"Reload1" action:@selector(reload1) frame:CGRectMake(180, 0, 80, 40)];
    UIButton *reloadButton2 = [self createButton:@"Reload2" action:@selector(reload2) frame:CGRectMake(270, 0, 80, 40)];
    UIButton *reloadButton3 = [self createButton:@"Reload3" action:@selector(reload3) frame:CGRectMake(360, 0, 80, 40)];
    UIView *buttonPanel = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 500, 40)];
    [buttonPanel addSubview:reloadButton];
    [buttonPanel addSubview:reloadButton0];
    [buttonPanel addSubview:reloadButton1];
    [buttonPanel addSubview:reloadButton2];
    [buttonPanel addSubview:reloadButton3];
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:buttonPanel];
}

- (UIButton *)createButton:(NSString *)title action:(SEL)action frame:(CGRect)frame {
    UIButton *button = [[UIButton alloc] initWithFrame:frame];
    [button setTitle:title forState:UIControlStateNormal];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [button setBackgroundColor:[UIColor lightGrayColor]];
    button.layer.cornerRadius = 5;
    [button addTarget:self action:action forControlEvents:UIControlEventTouchUpInside];
    return button;
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    CGFloat topPosition = CGRectGetMaxY(self.navigationController.navigationBar.frame);
    _contentView.frame = CGRectMake(0, topPosition, self.view.frame.size.width, self.view.frame.size.height - topPosition);
}


- (void)reload {
    [_contentView reloadGraphics];
}

- (void)reload0 {
    [_contentView reloadGraphicsAtIndex:0];
}

- (void)reload1 {
    [_contentView reloadGraphicsAtIndex:1];
}

- (void)reload2 {
    [_contentView reloadGraphicsAtIndex:2];
}

- (void)reload3 {
    [_contentView reloadGraphicsAtIndex:3];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
