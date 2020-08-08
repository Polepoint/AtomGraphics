//
// Created by neo on 2018/12/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JSCanvasContext.h"

@protocol JSImageBitmapExport <JSExport>

@property(nonatomic, assign) CGFloat width;

@property(nonatomic, assign) CGFloat height;

@property(nonatomic, strong) NSString *src;

@property(nonatomic, assign) JSValue *onload;

@end

@interface JSImageBitmap : NSObject <JSImageBitmapExport>

- (instancetype)init;

- (AtomGraphics::ImageBitmap *)getImageBitmap;

- (void)destroy;

@end