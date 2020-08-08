//
// Created by neo on 2019-09-16.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "HanabiDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation HanabiDemoViewController {

}

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent hanabiJSContent]];
}


@end
