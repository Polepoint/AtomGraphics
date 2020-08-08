//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "JSCanvasContext.h"
#import "JSCanvas.h"
#import "JSImageBitmap.h"
#import "CSSParser.h"
#import "JSImageData.h"
#import "JSCanvasStyle.h"

using namespace AtomGraphics;

@implementation JSCanvasContext {
    std::shared_ptr<CanvasRenderingContext2D> _canvasContext2d;
    NSArray<NSNumber *> *_lineDash;
    float _lineDashOffset;
}

- (instancetype)initWithCanvasContext:(std::shared_ptr<CanvasRenderingContext2D>)context2d {
    self = [super init];
    if (self) {
        _canvasContext2d = context2d;
    }

    return self;
}

- (int)shadowBlur {
    return 0;
}

- (void)setShadowBlur:(int)shadowBlur {
    _canvasContext2d->setShadowBlur(shadowBlur);
}

- (NSString *)shadowColor {
    return nil;
}

- (void)setShadowColor:(NSString *)shadowColor {
    Color4F colorStyle = CSSParser::parseCSSColor([shadowColor cString]);
    _canvasContext2d->setShadowColor(colorStyle);
}

- (float)shadowOffsetX {
    return 0;
}

- (void)setShadowOffsetX:(float)shadowOffsetX {
    _canvasContext2d->setShadowOffsetX(shadowOffsetX);
}

- (float)shadowOffsetY {
    return 0;
}

- (void)setShadowOffsetY:(float)shadowOffsetY {
    _canvasContext2d->setShadowOffsetY(shadowOffsetY);
}


- (id)fillStyle {
    return @"";
}

- (void)setFillStyle:(JSValue *)fillStyle {
    if ([fillStyle isNull] || [fillStyle isUndefined]) {
        _canvasContext2d->setFillStyle(Color4F::BLACK);
    } else if ([fillStyle isString]) {
        Color4F colorStyle = CSSParser::parseCSSColor([[fillStyle toString] cStringUsingEncoding:NSUTF8StringEncoding]);
        _canvasContext2d->setFillStyle(colorStyle);
    } else {
        id styleObj = [fillStyle toObjectOfClass:[JSCanvasStyle class]];
        if (styleObj) {
            JSCanvasStyle *jsCanvasFillStyle = (JSCanvasStyle *) styleObj;
            _canvasContext2d->setFillStyle(*[jsCanvasFillStyle getCanvasStyle]);
        }
    }
}

- (id)strokeStyle {
    return @"";
}

- (void)setStrokeStyle:(JSValue *)strokeStyle {
    if ([strokeStyle isNull] || [strokeStyle isUndefined]) {
        _canvasContext2d->setStrokeStyle(Color4F::BLACK);
    } else if ([strokeStyle isString]) {
        Color4F colorStyle = CSSParser::parseCSSColor([[strokeStyle toString] cStringUsingEncoding:NSUTF8StringEncoding]);
        _canvasContext2d->setStrokeStyle(colorStyle);
    } else {
        id styleObj = [strokeStyle toObjectOfClass:[JSCanvasStyle class]];
        if (styleObj) {
            JSCanvasStyle *jsCanvasFillStyle = (JSCanvasStyle *) styleObj;
            _canvasContext2d->setStrokeStyle(*[jsCanvasFillStyle getCanvasStyle]);
        }
    }
}


- (void)beginPath {
    _canvasContext2d->beginPath();
}

- (void)closePath {
    _canvasContext2d->closePath();
}

- (void)arc {
    NSArray *arguments = [JSContext currentArguments];
    int argsCount = [arguments count];
    if (argsCount >= 5) {
        double x = [arguments[0] toDouble];
        double y = [arguments[1] toDouble];
        double r = [arguments[2] toDouble];
        double sAngle = [arguments[3] toDouble];
        double eAngle = [arguments[4] toDouble];
        bool counterclockwise = argsCount == 6 ? [arguments[5] toBool] : false;
        _canvasContext2d->arc(static_cast<float>(x), static_cast<float>(y), static_cast<float>(r),
                              static_cast<float>(sAngle), static_cast<float>(eAngle), counterclockwise);
    }
}

- (bool)isPointInPath {
    NSArray *arguments = [JSContext currentArguments];
    int argsCount = [arguments count];
    if (argsCount >= 2) {
        return _canvasContext2d->isPointInPath(
                static_cast<float>([arguments[0] toDouble]),
                static_cast<float>([arguments[1] toDouble]));
    }

    return false;
}


- (void)stroke {
    _canvasContext2d->stroke();
}

- (void)fill:(JSValue *)fillRule {
    if ([fillRule isUndefined]) {
        _canvasContext2d->fill();
    } else {
        WindRule rule;
        if (parseFillRule([[fillRule toString] cString], rule)) {
            _canvasContext2d->fill(rule);
        } else {
            _canvasContext2d->fill();
        }
    }
}

- (void)save {
    _canvasContext2d->save();
}

- (void)restore {
    _canvasContext2d->restore();
}

- (void)moveTo:(double)x :(double)y {
    _canvasContext2d->moveTo(static_cast<float>(x), static_cast<float>(y));
}

- (void)lineTo:(double)x :(double)y {
    _canvasContext2d->lineTo(static_cast<float>(x), static_cast<float>(y));
}

- (double)lineWidth {
    return 0;
}

- (void)setLineWidth:(double)lineWidth {
    _canvasContext2d->setLineWidth(static_cast<float>(lineWidth));
}

- (NSString *)lineCap {
    return @"";
}

- (void)setLineCap:(NSString *)lineCapStr {
    LineCap lineCap;
    if (parseLineCap(lineCapStr.UTF8String, lineCap)) {
        _canvasContext2d->setLineCap(lineCap);
    }
}

- (NSString *)lineJoin {
    return @"";
}

- (void)setLineJoin:(NSString *)lineJoinStr {
    LineJoin lineJoin;
    if (parseLineJoin(lineJoinStr.UTF8String, lineJoin)) {
        _canvasContext2d->setLineJoin(lineJoin);
    }
}

- (float)miterLimit {
    return 0;
}

- (void)setMiterLimit:(float)miterLimit {
    _canvasContext2d->setMiterLimit(miterLimit);
}

- (void)rect:(float)x :(float)y :(float)width :(float)height {
    if (width && height) {
        _canvasContext2d->rect(x, y, width, height);
    }
}

- (void)fillRect:(float)x :(float)y :(float)width :(float)height {
    _canvasContext2d->fillRect(x, y, width, height);
}

- (void)strokeRect:(float)x :(float)y :(float)width :(float)height {
    _canvasContext2d->strokeRect(x, y, width, height);
}

- (void)clearRect:(float)x :(float)y :(float)width :(float)height {
    _canvasContext2d->clearRect(x, y, width, height);
}

- (void)clip {
    _canvasContext2d->clip();
}

- (void)quadraticCurveTo:(float)cpx :(float)cpy :(float)x :(float)y {
    _canvasContext2d->quadraticCurveTo(cpx, cpy, x, y);
}

- (void)arcTo:(float)x1 :(float)y1 :(float)x2 :(float)y2 :(float)r {
    _canvasContext2d->arcTo(x1, y1, x2, y2, r);
}

- (void)scale:(float)scaleWidth :(float)scaleHeight {
    _canvasContext2d->scale(scaleWidth, scaleHeight);
}

- (void)rotate:(double)angle {
    _canvasContext2d->rotate(static_cast<float>(angle));
}

- (void)translate:(float)x :(float)y {
    _canvasContext2d->translate(x, y);
}

- (void)transform:(float)a :(float)b :(float)c :(float)d :(float)e :(float)f {
    _canvasContext2d->transform(a, b, c, d, e, f);
}

- (void)setTransform:(double)a :(double)b :(double)c :(double)d :(double)e :(double)f {
    _canvasContext2d->setTransform(static_cast<float>(a), static_cast<float>(b), static_cast<float>(c),
                                   static_cast<float>(d), static_cast<float>(e), static_cast<float>(f));
}

- (void)setLineDash:(NSArray<NSNumber *> *)segments {
    _lineDash = segments;

    std::vector<float> dashArray;
    for (NSNumber *segment in segments) {
        dashArray.push_back(segment.floatValue);
    }

    _canvasContext2d->setLineDash(dashArray);
}

- (NSArray<NSNumber *> *)getLineDash {
    return _lineDash;
}

- (float)lineDashOffset {
    return _lineDashOffset;
}

- (void)setLineDashOffset:(float)lineDashOffset {
    _lineDashOffset = lineDashOffset;
    _canvasContext2d->setLineDashOffset(lineDashOffset);
}

- (void)fillText {
    NSArray *args = [JSContext currentArguments];
    int argCount = [args count];
    if (argCount >= 3) {
        NSString *text = [args[0] toString];
        double x = [args[1] toDouble];
        double y = [args[2] toDouble];
        bool hasMaxWith = argCount >= 4;
        float maxWidth = hasMaxWith ? (float) [args[3] toDouble] : 0.f;
        const char *buffer = [text cStringUsingEncoding:NSUTF8StringEncoding];
        _canvasContext2d->fillText(base::String(buffer, strlen(buffer)), static_cast<float>(x), static_cast<float>(y), hasMaxWith ? &maxWidth : nullptr);
    }
}

- (NSString *)font {
    return [NSString stringWithCString:_canvasContext2d->getFont().data() encoding:NSUTF8StringEncoding];
}

- (void)setFont:(NSString *)font {
    if ([font containsString:@","]) {
        font = [font componentsSeparatedByString:@","][0];
    }
    _canvasContext2d->setFont(std::string([font UTF8String]));
}

- (NSString *)textAlign {
    return @"";
}

- (void)setTextAlign:(NSString *)textAlign {
    TextAlign align;
    if (parseTextAlign(textAlign.cString, align)) {
        _canvasContext2d->setTextAlign(align);
    }
}

- (NSString *)textBaseline {
    return @"";
}

- (void)setTextBaseline:(NSString *)textBaseline {
    TextBaseline baseline;
    if (parseTextBaseline(textBaseline.cString, baseline)) {
        _canvasContext2d->setTextBaseline(baseline);
    }
}

- (void)strokeText {
    NSArray *args = [JSContext currentArguments];
    int argCount = [args count];
    if (argCount >= 3) {
        NSString *text = [args[0] toString];
        double x = [args[1] toDouble];
        double y = [args[2] toDouble];
        float maxWidth = argCount >= 4 ? (float) [args[3] toDouble] : 0.f;
        const char *buffer = [text cStringUsingEncoding:NSUTF8StringEncoding];
        _canvasContext2d->strokeText(base::String(buffer, strlen(buffer)), static_cast<float>(x), static_cast<float>(y), &maxWidth);
    }
}

- (double)globalAlpha {
    return 0;
}

- (void)setGlobalAlpha:(double)globalAlpha {
    _canvasContext2d->setGlobalAlpha(static_cast<float>(globalAlpha));
}

- (NSString *)globalCompositeOperation {
    return @"";
}

- (void)setGlobalCompositeOperation:(NSString *)globalCompositeOperation {
    CompositeOperator operation;
    BlendMode blendMode;
    parseCompositeAndBlendOperator([globalCompositeOperation UTF8String], operation, blendMode);
    _canvasContext2d->setGlobalCompositeOperation(operation, blendMode);
}

- (void)bezierCurveTo:(float)cp1x :(float)cp1y :(float)cp2x :(float)cp2y :(float)x :(float)y {
    _canvasContext2d->bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

- (JSCanvasStyle *)createPattern:(JSValue *)source :(NSString *)pattern {
    std::shared_ptr<CanvasImageSource> imageSource;
    id sourceObj = [source toObjectOfClass:[JSImageBitmap class]];
    if (sourceObj) {
        imageSource.reset(new CanvasImageSource([(JSImageBitmap *) sourceObj getImageBitmap]));
    } else {
        sourceObj = [source toObjectOfClass:[JSCanvas class]];
        if (!sourceObj) {
            return nil;
        }
        imageSource.reset(new CanvasImageSource([(JSCanvas *) sourceObj getCanvasNode]));
    }

    std::shared_ptr<Pattern> canvasPattern;

    bool repeatX = false;
    bool repeatY = false;
    if (CSSParser::parseRepetitionType([pattern UTF8String], repeatX, repeatY)) {
        canvasPattern = std::make_shared<Pattern>(imageSource.get(), repeatX, repeatY);
    } else {
        canvasPattern = std::make_shared<Pattern>();
    }
    std::unique_ptr<CanvasStyle> canvasStyle(new CanvasStyle(canvasPattern));
    return [[JSCanvasStyle alloc] initWithCanvasFillStyle:std::move(canvasStyle)];
}

- (JSCanvasStyle *)createLinearGradient:(float)x0 :(float)y0 :(float)x1 :(float)y1 {
    std::shared_ptr<Gradient> linearGradient = std::make_shared<Gradient>(x0, y0, x1, y1);
    std::unique_ptr<CanvasStyle> canvasStyle(new CanvasStyle(std::move(linearGradient)));
    return [[JSCanvasStyle alloc] initWithCanvasFillStyle:std::move(canvasStyle)];
}

- (JSCanvasStyle *)createRadialGradient:(float)x0 :(float)y0 :(float)r0 :(float)x1 :(float)y1 :(float)r1 {
    std::shared_ptr<Gradient> radialGradient = std::make_shared<Gradient>(x0, y0, r0, x1, y1, r1);
    std::unique_ptr<CanvasStyle> canvasStyle(new CanvasStyle(std::move(radialGradient)));
    return [[JSCanvasStyle alloc] initWithCanvasFillStyle:std::move(canvasStyle)];
}

- (JSImageData *)getImageData:(int)x :(int)y :(int)width :(int)height {
    return [[JSImageData alloc] initWithCanvasImageData:std::move(_canvasContext2d->getImageData(x, y, width, height))];
}

- (void)drawImage {
    NSArray *args = [JSContext currentArguments];
    NSInteger argCount = [args count];

    if (argCount > 0) {
        std::unique_ptr<CanvasImageSource> imageSource;
        id element = [args[0] toObjectOfClass:[JSCanvas class]];
        if (element) {
            auto node = [(JSCanvas *) element getCanvasNode];
            imageSource.reset(new CanvasImageSource(node));
        } else {
            element = [args[0] toObjectOfClass:[JSImageBitmap class]];
            if (!element) {
                return;
            }
            auto imageBitmap = [(JSImageBitmap *) element getImageBitmap];
            imageSource.reset(new CanvasImageSource(imageBitmap));
        }

        if (argCount == 3) {
            float x = (float) [args[1] toDouble];
            float y = (float) [args[2] toDouble];
            _canvasContext2d->drawImage(imageSource.get(), x, y);
        } else if (argCount == 5) {
            float x = (float) [args[1] toDouble];
            float y = (float) [args[2] toDouble];
            float width = (float) [args[3] toDouble];
            float height = (float) [args[4] toDouble];
            _canvasContext2d->drawImage(imageSource.get(), x, y, width, height);
        } else if (argCount == 9) {
            float sx = (float) [args[1] toDouble];
            float sy = (float) [args[2] toDouble];
            float swidth = (float) [args[3] toDouble];
            float sheight = (float) [args[4] toDouble];
            float x = (float) [args[5] toDouble];
            float y = (float) [args[6] toDouble];
            float width = (float) [args[7] toDouble];
            float height = (float) [args[8] toDouble];
            _canvasContext2d->drawImage(imageSource.get(), sx, sy, swidth, sheight, x, y, width, height);
        }
    }
}

- (JSImageData *)createImageData {
    NSArray *args = [JSContext currentArguments];
    int imageWidth = 0, imageHeight = 0;
    scoped_refptr<ImageData> imageData;
    if ([args count] == 1) {
        JSValue *value = args[0];
        if ([value.toObject isKindOfClass:[JSImageData class]]) {
            JSImageData *jsImageData = (JSImageData *) value.toObject;
            IntSize imgSize = jsImageData.getCanvasImageData->size();
            imageData = ImageData::Create(imgSize, 4 * imgSize.width);
        }
    } else if ([args count] == 2) {
        imageWidth = [args[0] toDouble];
        imageHeight = [args[1] toDouble];
        imageData = ImageData::Create({imageWidth, imageHeight}, 4 * imageWidth);
    }

    if (imageData) {
        return [[JSImageData alloc] initWithCanvasImageData:std::move(imageData)];

    }

    return nullptr;
}

- (void)putImageData {
    NSArray *args = [JSContext currentArguments];
    int argCount = [args count];
    if (argCount == 3) { //canvasImageDataJavaScriptInterface ,x , y
        JSValue *canvasImageData = args[0];
        int x = [args[1] toInt32];
        int y = [args[2] toInt32];

        if ([canvasImageData.toObject isKindOfClass:[JSImageData class]]) {
            JSImageData *jsImageData = (JSImageData *) canvasImageData.toObject;
            ImageData *imageData = jsImageData.getCanvasImageData;
            _canvasContext2d->putImageData(imageData, x, y);
        }
    } else if (argCount == 7) { //imageData, x,y ,and optional dirtyX,dirtyY,dirtyWidth,dirtyHeight
        JSValue *imageData = args[0];
        int x = [args[1] toInt32];
        int y = [args[2] toInt32];
        int dirtyX = argCount > 3 ? [args[3] toInt32] : 0;
        int dirtyY = argCount > 4 ? [args[4] toInt32] : 0;
        int dirtyWidth = argCount > 5 ? [args[5] toInt32] : 0;
        int dirtyHeight = argCount > 6 ? [args[6] toInt32] : 0;
        if ([imageData.toObject isKindOfClass:[JSImageData class]]) {
            JSImageData *jsImageData = (JSImageData *) imageData.toObject;
            ImageData *data = jsImageData.getCanvasImageData;
            _canvasContext2d->putImageData(data, x, y, dirtyX, dirtyY, dirtyWidth, dirtyHeight);
        }
    }
}


- (NSDictionary *)measureText:(NSString *)text {
    float width = 0.f;
    if (text) {
        const char *buffer = [text cStringUsingEncoding:NSUTF8StringEncoding];
        base::String str = base::String(buffer, strlen(buffer));
        TextMetrics textMetrics = _canvasContext2d->measureText(str);
        width = textMetrics.getWidth();
    }

    return @{@"width": @(width)};
}


@end