//
// Created by neo on 2019-09-20.
// Copyright (c) 2019 neo. All rights reserved.
//

#import "PixelsGraphicDemoViewController.h"
#import "GlobalResourceContent.h"


@implementation PixelsGraphicDemoViewController {

}

- (void)loadJSContent {
    [self.graphicsView evaluateScript:[GlobalResourceContent pixelsGraphicJSContent]];
}


@end