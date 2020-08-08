//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "ImageBitmap.h"
#import "CanvasRenderingContext2D.h"

@class JSImageData;
@class JSCanvasStyle;

@protocol JSCanvasContextExport <JSExport>

@property(nonatomic) JSValue *fillStyle;

@property(nonatomic) JSValue *strokeStyle;

@property(nonatomic, assign) int shadowBlur;

@property(nonatomic, copy) NSString *shadowColor;

@property(nonatomic, assign) float shadowOffsetX;

@property(nonatomic, assign) float shadowOffsetY;

@property(nonatomic, assign) double lineWidth;

@property(nonatomic, copy) NSString *lineCap;

@property(nonatomic, copy) NSString *lineJoin;

@property(nonatomic, assign) float lineDashOffset;

@property(nonatomic, assign) float miterLimit;

@property(nonatomic, assign) double globalAlpha;

@property(nonatomic, copy) NSString *globalCompositeOperation;

// e.g. "40px Arial";
@property(nonatomic, copy) NSString *font;

@property(nonatomic, copy) NSString *textAlign;

@property(nonatomic, copy) NSString *textBaseline;

- (void)beginPath;

- (void)closePath;

- (void)arc;

- (bool)isPointInPath;

- (void)stroke;

- (void)fill:(JSValue *)fillRule;

- (void)save;

- (void)restore;

- (void)moveTo:(double)x :(double)y;

- (void)lineTo:(double)x :(double)y;

- (void)rect:(float)x :(float)y :(float)width :(float)height;

- (void)fillRect:(float)x :(float)y :(float)width :(float)height;

- (void)strokeRect:(float)x :(float)y :(float)width :(float)height;

- (void)clearRect:(float)x :(float)y :(float)width :(float)height;

- (void)clip;

- (void)quadraticCurveTo:(float)cpx :(float)cpy :(float)x :(float)y;

- (void)bezierCurveTo:(float)cp1x :(float)cp1y :(float)cp2x :(float)cp2y :(float)x :(float)y;

- (void)arcTo:(float)x1 :(float)y1 :(float)x2 :(float)y2 :(float)r;

- (void)scale:(float)scaleWidth :(float)scaleHeight;

- (void)rotate:(double)angle;

- (void)translate:(float)x :(float)y;

- (void)transform:(float)a :(float)b :(float)c :(float)d :(float)e :(float)f;

- (void)setTransform:(double)a :(double)b :(double)c :(double)d :(double)e :(double)f;

- (void)setLineDash:(NSArray<NSNumber *> *)segments;

- (NSArray<NSNumber *> *)getLineDash;

- (void)fillText;

- (void)strokeText;

- (JSCanvasStyle *)createPattern:(JSValue *)source :(NSString *)pattern;

- (JSCanvasStyle *)createLinearGradient:(float)x0 :(float)y0 :(float)x1 :(float)y1;

- (JSCanvasStyle *)createRadialGradient:(float)x0 :(float)y0 :(float)r0 :(float)x1 :(float)y1 :(float)r1;

- (JSImageData *)getImageData:(int)x :(int)y :(int)width :(int)height;

- (void)drawImage;

//内部通过jsContext获取参数
- (JSImageData *)createImageData;

- (void)putImageData;
//:(JSValue *)imageData :(int)x :(int)y :(int)dirtyX :(int)dirtyY :(int)dirtyWidth :(int)dirtyHeight;

- (NSDictionary *)measureText:(NSString *)text;

@end

@interface JSCanvasContext : NSObject <JSCanvasContextExport>

- (instancetype)initWithCanvasContext:(std::shared_ptr<AtomGraphics::CanvasRenderingContext2D>)context2d;

@end