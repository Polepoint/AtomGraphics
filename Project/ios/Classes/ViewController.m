//
//  ViewController.m
//  AtomGraphics
//
//  Created by neo on 2017/11/15.
//  Copyright (c) 2017 neo. All rights reserved.
//

#import <AtomGraphics/GraphicsJavaScriptCore.h>
#import "ViewController.h"
#import "MultiGraphicsDemoViewController.h"
#import "TadpolesDemoViewController.h"
#import "CollectionViewCell.h"
#import "LoadingDemoViewController.h"
#import "Loading2DemoViewController.h"
#import "FlowerDemoViewController.h"
#import "HeartDemoViewController.h"
#import "ClockDemoViewController.h"
#import "ColorfulTextDemoViewController.h"
#import "HanabiDemoViewController.h"
#import "PixelsGraphicDemoViewController.h"
#import "GalaxyDemoViewController.h"
#import <PerformanceMonitor/PerformanceMonitor.h>

static NSString *kTableCellReuseIdentifier = @"__tableview_resuse_identifier___";

static const NSString *kDictKeyTitle = @"title";
static const NSString *kDictKeyViewControllerClass = @"class";

static NSArray *sDemoList;

@interface ViewController () <UICollectionViewDataSource, UICollectionViewDelegateFlowLayout>

@property(nonatomic, strong) UIView *contentView;
@property(nonatomic, strong) UICollectionView *collectionView;
@property(nonatomic, strong) UIButton *resetButton;
@property(nonatomic, strong) UIButton *toggleMonitorButton;
@property(nonatomic, strong) PerformanceMonitor *monitor;

@end

@implementation ViewController {

}

+ (void)initialize {
    [super initialize];
    sDemoList = @[
            @{
                    kDictKeyTitle: @"Multiple Graphics",
                    kDictKeyViewControllerClass: [MultiGraphicsDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Tadpoles",
                    kDictKeyViewControllerClass: [TadpolesDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Loading",
                    kDictKeyViewControllerClass: [LoadingDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Loading2",
                    kDictKeyViewControllerClass: [Loading2DemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Flower",
                    kDictKeyViewControllerClass: [FlowerDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Heart",
                    kDictKeyViewControllerClass: [HeartDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Clock",
                    kDictKeyViewControllerClass: [ClockDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Colorful Text",
                    kDictKeyViewControllerClass: [ColorfulTextDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Hanabi",
                    kDictKeyViewControllerClass: [HanabiDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Pixels-Graphic",
                    kDictKeyViewControllerClass: [PixelsGraphicDemoViewController class]
            },
            @{
                    kDictKeyTitle: @"Galaxy",
                    kDictKeyViewControllerClass: [GalaxyDemoViewController class]
            },
    ];
}


- (void)viewDidLoad {
    [super viewDidLoad];

    UIImageView *backgroundView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"playground"]];
    backgroundView.frame = self.view.bounds;
    backgroundView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    [self.view addSubview:backgroundView];

    _contentView = [[UIView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:_contentView];

    [self loadScript];

    _collectionView = [[UICollectionView alloc] initWithFrame:_contentView.bounds collectionViewLayout:[UICollectionViewFlowLayout new]];
    _collectionView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    [_collectionView registerClass:[CollectionViewCell class] forCellWithReuseIdentifier:kTableCellReuseIdentifier];
    _collectionView.dataSource = self;
    _collectionView.delegate = self;
    _collectionView.backgroundColor = [UIColor clearColor];
    [_contentView addSubview:_collectionView];

    CGFloat buttonWidth = 150;
    CGFloat gap = 10;
    _resetButton = [[UIButton alloc] initWithFrame:CGRectMake(_collectionView.frame.size.width - buttonWidth - gap, gap, buttonWidth, 40)];
    [_resetButton addTarget:self action:@selector(onReset) forControlEvents:UIControlEventTouchUpInside];
    [_resetButton setTitle:@"Reset Context" forState:UIControlStateNormal];
    [_resetButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [_resetButton setBackgroundColor:[[UIColor lightGrayColor] colorWithAlphaComponent:0.7]];
    _resetButton.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin;
    [_contentView addSubview:_resetButton];


    _toggleMonitorButton = [[UIButton alloc] initWithFrame:
            CGRectMake(_collectionView.frame.size.width - buttonWidth - gap,
                       CGRectGetMaxY(_resetButton.frame) + gap,
                       buttonWidth,
                       40)];
    [_toggleMonitorButton addTarget:self action:@selector(toggleMonitor) forControlEvents:UIControlEventTouchUpInside];
    [_toggleMonitorButton setTitle:@"Toggle Monitor" forState:UIControlStateNormal];
    [_toggleMonitorButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
    [_toggleMonitorButton setBackgroundColor:[[UIColor lightGrayColor] colorWithAlphaComponent:0.7]];
    _toggleMonitorButton.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin;
    [_contentView addSubview:_toggleMonitorButton];


}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    if (@available(iOS 11.0, *)) {
        _contentView.frame = UIEdgeInsetsInsetRect(self.view.bounds, self.view.safeAreaInsets);
    } else {
        CGFloat topPosition = CGRectGetMaxY(self.navigationController.navigationBar.frame);
        _contentView.frame = CGRectMake(0, topPosition, self.view.frame.size.width, self.view.frame.size.height - topPosition);
    }
}


- (void)onReset {
    [[GraphicsJavaScriptCore sharedJavaScriptCore] reset];
    [self loadScript];

    UIAlertController *alertVc = [UIAlertController alertControllerWithTitle:@"Reset JSContext Complete!" message:nil preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *sureBtn = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil];
    [alertVc addAction:sureBtn];
    [self presentViewController:alertVc animated:YES completion:nil];
}

- (void)toggleMonitor {
    if (!_monitor) {
        _monitor = [PerformanceMonitor new];
    }

    if (_monitor.shown) {
        [_monitor hide];
    } else {
        [_monitor show];
    }
}

- (void)loadScript {
    GraphicsJavaScriptCore *javaScriptCore = [GraphicsJavaScriptCore sharedJavaScriptCore];
    [javaScriptCore runScript:@""];
}

#pragma mark - UICollectionViewDataSource Methods


- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return sDemoList.count;
}

- (__kindof UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    CollectionViewCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:kTableCellReuseIdentifier forIndexPath:indexPath];
    cell.title = sDemoList[indexPath.row][kDictKeyTitle];
    return cell;
}


#pragma mark - UICollectionViewDelegate Methods


- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    UIViewController *controller = [sDemoList[indexPath.row][kDictKeyViewControllerClass] new];
    controller.title = sDemoList[indexPath.row][kDictKeyTitle];
    [self.navigationController pushViewController:controller animated:YES];
}

- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout referenceSizeForHeaderInSection:(NSInteger)section {
    return CGSizeMake(0, 100);
}

- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout insetForSectionAtIndex:(NSInteger)section {
    return UIEdgeInsetsMake(5, 20, 5, 20);
}


- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    return CGSizeMake(300, 50);
}

@end
