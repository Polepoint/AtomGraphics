//
//  ViewController.m
//  AtomGraphics
//
//  Created by neo on 2017/11/15.
//  Copyright © 2017年 neo. All rights reserved.
//

#import "ViewController.h"
#import "AtomGraphicsView.h"
#import "FPSLabel.h"
#import "AtomGraphicsGCanvasView.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    UIView *contentView = [[AtomGraphicsView alloc] initWithFrame:self.view.bounds];
//    UIView *contentView = [[AtomGraphicsGCanvasView alloc] initWithFrame:self.view.bounds];
    contentView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    contentView.backgroundColor = [UIColor orangeColor];
    [self.view addSubview:contentView];
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:[[FPSLabel alloc] initWithFrame:CGRectMake(0, 0, 100, 40)]];
    // Do any additional setup after loading the view, typically from a nib.
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
