//
// Created by neo on 2019-09-19.
//

#import "JSCanvasStyle.h"
#import "CSSParser.h"

using namespace AtomGraphics;

@implementation JSCanvasStyle {
    std::unique_ptr<CanvasStyle> _canvasFillStyle;
}

- (instancetype)initWithCanvasFillStyle:(std::unique_ptr<CanvasStyle>)canvasFillStyle {
    self = [super init];
    if (self) {
        _canvasFillStyle = std::move(canvasFillStyle);
    }
    return self;
}

- (CanvasStyle *)getCanvasStyle {
    return _canvasFillStyle.get();
}

- (void)addColorStop:(float)stop :(NSString *)colorStr {
    _canvasFillStyle->getCanvasGradient()->addColorStop(stop, CSSParser::parseCSSColor([colorStr UTF8String]));
}

@end
