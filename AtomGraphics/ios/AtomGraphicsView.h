//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "JSPageScope.h"

@class AtomGraphicsView;
@class GraphicsConfiguration;

@protocol AtomGraphicsViewDelegate <NSObject>

@optional

- (void)graphicsView:(AtomGraphicsView *)graphicsView viewportDidChange:(CGRect)viewport;

@end

@interface AtomGraphicsView : UIView {

}

@property(nonatomic, weak) id <AtomGraphicsViewDelegate> delegate;

- (void)evaluateScript:(NSString *)script;

- (void)callJSHandler:(NSString *)handlerName;

- (void)callJSHandler:(NSString *)handlerName parameters:(id)params;

- (void)registerNativeHandler:(NSString *)handlerName handlerCallback:(ScopeHandler)callback;

@end

