//
// Created by neo on 2018/11/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "GraphicsPage.h"

struct ContentViewInfo {
    long pageID;

    ContentViewInfo(long page_id) : pageID(page_id) {};
};

@protocol GraphicsContentViewDelegate <NSObject>

- (void)didFinishInitialization:(ContentViewInfo)info;

@end


@interface GraphicsContentView : UIView

@property(nonatomic, weak) id <GraphicsContentViewDelegate> delegate;

- (void)applyTransaction:(AtomGraphics::Transaction *)transaction;

- (void)didUpdateVisibleRect:(const CGRect &)rect pageBounds:(const CGRect &)bounds transformScale:(float)transformScale;

- (void)destroy;

@end