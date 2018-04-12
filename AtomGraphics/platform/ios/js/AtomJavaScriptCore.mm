//
// Created by neo on 2018/4/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <JavaScriptCore/JavaScriptCore.h>
#import "AtomJavaScriptCore.h"


//TODO: improve implement and move to AtomTypes
static inline Color4F colorWithRgba(NSString *rgba) {
    Color4F color;
    if (!rgba) {
        return color;
    }
    rgba = [rgba stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    NSString *regex = @"\\d+([.]\\d+)?";
    NSError *error = nil;
    NSRegularExpression *regular = [NSRegularExpression regularExpressionWithPattern:regex options:NSRegularExpressionCaseInsensitive error:&error];
    NSArray *matches = [regular matchesInString:rgba options:NSMatchingReportProgress range:NSMakeRange(0, [rgba length])];
    NSString *red, *green, *blue, *alpha;
    if (!error && [matches count] > 2) {
        red = [rgba substringWithRange:[matches[0] range]];
        green = [rgba substringWithRange:[matches[1] range]];
        blue = [rgba substringWithRange:[matches[2] range]];
        if ([matches count] > 3) {
            alpha = [rgba substringWithRange:[matches[3] range]];
        } else {
            alpha = @"1.0";
        }
        if ([rgba hasPrefix:@"rgb"]) {
            color.r = red.floatValue / 255;
            color.g = green.floatValue / 255;
            color.b = blue.floatValue / 255;
            color.a = alpha.floatValue;
        } else {
            color.r = red.floatValue;
            color.g = green.floatValue;
            color.b = blue.floatValue;
            color.a = alpha.floatValue;
        }
    }

    return color;
}

@protocol CanvasContextJavaScriptInterfaceExport <JSExport>

@property(nonatomic, copy) NSString *fillStyle;

- (void)beginPath;

- (void)arc:(float)x :(float)y :(float)r :(float)sAngle :(float)eAngle :(bool)counterclockwise;

- (void)stroke;

- (void)fill;

@end

@interface CanvasContextJavaScriptInterface : NSObject <CanvasContextJavaScriptInterfaceExport>

@end

@implementation CanvasContextJavaScriptInterface {
    CanvasContext2d *_canvasContext2d;
}

- (instancetype)initWithCanvasContext:(CanvasContext2d *)pCanvasContext2d {
    self = [super init];
    if (self) {
        _canvasContext2d = pCanvasContext2d;
    }

    return self;
}

- (NSString *)fillStyle {
    return @"";
}


- (void)setFillStyle:(NSString *)fillStyle {
    _canvasContext2d->setFillStyle(colorWithRgba(fillStyle));
}

- (void)beginPath {
    _canvasContext2d->beginPath();
}

- (void)arc:(float)x :(float)y :(float)r :(float)sAngle :(float)eAngle :(bool)counterclockwise {
    _canvasContext2d->arc(x, y, r, sAngle, eAngle, counterclockwise);
}


- (void)stroke {
    _canvasContext2d->stroke();
}

- (void)fill {
    _canvasContext2d->fill();
}

@end

@protocol CanvasJavaScriptInterfaceExport <JSExport>

- (CanvasContextJavaScriptInterface *)getContext:(NSString *)contentType;

@end

@interface CanvasJavaScriptInterface : NSObject <CanvasJavaScriptInterfaceExport>

@end

@implementation CanvasJavaScriptInterface {
    CanvasNode *_canvasNode;
}

- (instancetype)initWithCanvasNode:(CanvasNode *)canvasNode {
    self = [super init];
    if (self) {
        _canvasNode = canvasNode;
    }

    return self;
}

- (CanvasContextJavaScriptInterface *)getContext:(NSString *)contentType {
    return [[CanvasContextJavaScriptInterface alloc] initWithCanvasContext:_canvasNode->getContext2d()];
}


@end


@interface AtomJavaScriptCore ()

@property(nonatomic, strong) JSContext *jsContext;

@property(nonatomic, strong) CanvasJavaScriptInterface *canvasJavaScriptInterface;

@end

@implementation AtomJavaScriptCore {

}

- (instancetype)initWithCanvasNode:(CanvasNode *)canvasNode {
    self = [super init];
    if (self) {
        _canvasJavaScriptInterface = [[CanvasJavaScriptInterface alloc] initWithCanvasNode:canvasNode];
//        TODO:
//        JSGlobalContextRef ctx = JSGlobalContextCreate()
//        _jsContext= [JSContext contextWithJSGlobalContextRef:ctx];
        NSString *jsCoreFilePath = [[NSBundle mainBundle] pathForResource:@"core" ofType:@"js"];
        if (jsCoreFilePath) {
            _jsContext = [[JSContext alloc] init];
            [_jsContext evaluateScript:[NSString stringWithContentsOfFile:jsCoreFilePath]];
            [self initGlobalContext];
        }
    }

    return self;
}


- (void)initGlobalContext {

    JSValue *AG = _jsContext[@"AG"];
    [AG  setValue:^() {
        return _canvasJavaScriptInterface;
    } forProperty:@"getRootCanvasNode"];
}

- (void)runScriptFile:(NSString *)path {
    if (path) {
        [_jsContext evaluateScript:[NSString stringWithContentsOfFile:path]];
    }
}

@end

