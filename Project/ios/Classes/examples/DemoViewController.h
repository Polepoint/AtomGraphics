//
// Created by neo on 2019-09-16.
// Copyright (c) 2019 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AtomGraphics/AtomGraphicsView.h>


@interface DemoViewController : UIViewController

@property(nonatomic, strong) AtomGraphicsView *graphicsView;

- (void)loadJSContent;

@end