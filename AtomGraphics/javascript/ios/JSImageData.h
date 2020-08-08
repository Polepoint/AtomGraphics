//
// Created by neo on 2019-09-19.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import <memory>
#import "ImageData.h"


@protocol CanvasImageDataExport <JSExport>

@property(nonatomic, assign) CGFloat width;

@property(nonatomic, assign) CGFloat height;

@property(nonatomic, strong) JSValue *data;

@end

@interface JSImageData : NSObject <CanvasImageDataExport>

- (instancetype)initWithCanvasImageData:(AtomGraphics::scoped_refptr<AtomGraphics::ImageData>)canvasImageData;

- (AtomGraphics::ImageData *)getCanvasImageData;

@end