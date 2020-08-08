//
// Created by neo on 2019-09-19.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
#import "CanvasStyle.h"

@protocol CanvasStyleExport <JSExport>

- (void)addColorStop:(float)stop :(NSString *)colorStr;

@end

@interface JSCanvasStyle : NSObject <CanvasStyleExport>

enum CanvasFillStyle {
    CanvasFillStyleUndefined = 0,
    CanvasFillStylePattern,
    CanvasFillStyleLinearGradient,
    CanvasFillStyleRadialGradient,
};

- (instancetype)initWithCanvasFillStyle:(std::unique_ptr<AtomGraphics::CanvasStyle>)canvasFillStyle;

- (AtomGraphics::CanvasStyle *)getCanvasStyle;

@end