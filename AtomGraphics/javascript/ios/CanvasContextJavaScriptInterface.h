//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomCanvasImage.h"
#import "AtomCanvasFillStyle.h"
#import "AtomCanvasContext2D.h"

@protocol CanvasImageJavaScriptInterfaceExport <JSExport>

@end

@interface CanvasImageJavaScriptInterface : NSObject <CanvasImageJavaScriptInterfaceExport>

- (instancetype)initWithCanvasImage:(AtomGraphics::AtomCanvasImage *)pCanvasImage;

- (AtomGraphics::AtomCanvasImage *)getCanvasImage;

@end

@protocol CanvasFillStyleJavaScriptInterfaceExport <JSExport>

- (void)addColorStop:(float)stop :(NSString *)colorStr;

@end

@interface CanvasFillStyleJavaScriptInterface : NSObject <CanvasFillStyleJavaScriptInterfaceExport>

enum CanvasFillStyle {
    CanvasFillStyleUndefined = 0,
    CanvasFillStylePattern,
    CanvasFillStyleLinearGradient,
    CanvasFillStyleRadialGradient,
};

- (instancetype)initWithAtomCanvasFillStyle:(AtomGraphics::AtomCanvasFillStyle *)pAtomCanvasFillStyle;

- (AtomGraphics::AtomCanvasFillStyle *)getCanvasFillStyle;

@end

@protocol CanvasContextJavaScriptInterfaceExport <JSExport>

@property(nonatomic) id fillStyle;

@property(nonatomic) id strokeStyle;

@property(nonatomic, assign) int shadowBlur;

@property(nonatomic, copy) NSString *shadowColor;

@property(nonatomic, assign) float shadowOffsetX;

@property(nonatomic, assign) float shadowOffsetY;

@property(nonatomic, assign) double lineWidth;

@property(nonatomic, copy) NSString *lineCap;

@property(nonatomic, copy) NSString *lineJoin;

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

- (void)stroke;

- (void)fill;

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

- (void)fillText;

- (void)strokeText;

- (CanvasFillStyleJavaScriptInterface *)createPattern:(CanvasImageJavaScriptInterface *)image :(NSString *)pattern;

- (CanvasFillStyleJavaScriptInterface *)createLinearGradient:(float)x0 :(float)y0 :(float)x1 :(float)y1;

- (CanvasFillStyleJavaScriptInterface *)createRadialGradient:(float)x0 :(float)y0 :(float)r0 :(float)x1 :(float)y1 :(float)r1;

- (CanvasImageJavaScriptInterface *)getImageData:(int)x :(int)y :(int)width :(int)height;

- (void)drawImage;

//内部通过jsContext获取参数
- (JSValue *)createImageData;

- (void)putImageData;
//:(JSValue *)imageData :(int)x :(int)y :(int)dirtyX :(int)dirtyY :(int)dirtyWidth :(int)dirtyHeight;

- (JSValue *)measureText;
@end

@interface CanvasContextJavaScriptInterface : NSObject <CanvasContextJavaScriptInterfaceExport>

- (instancetype)initWithCanvasContext:(AtomGraphics::CanvasContext2D *)pCanvasContext2d;

@end