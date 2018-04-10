//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <GCanvas/GCanvasViewProtocol.h>

@interface AtomGraphicsGCanvasView : UIView <GCanvasViewProtocol>


@property(nonatomic, strong) GLKView *glkview;
@property(nonatomic, assign) CGRect componetFrame;

/**
 * offscreen UIView<GCanvasViewProtocol>
 */
@property(nonatomic, assign, readonly) BOOL isOffscreen;

@property(nonatomic, assign) BOOL needChangeEAGLContenxt;

@end