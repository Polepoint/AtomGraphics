//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "CompositingView.h"

@implementation CompositingView {

}

- (instancetype)init {
    self = [super init];
    if (self) {
        self.layer.anchorPoint = CGPointZero;
    }
    
    return self;
}


@end