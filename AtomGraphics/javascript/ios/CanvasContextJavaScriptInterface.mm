//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "CanvasContextJavaScriptInterface.h"
#import "AtomConvert.h"
#import "CanvasJavaScriptInterface.h"

using namespace AtomGraphics;

@implementation CanvasImageJavaScriptInterface {
    AtomCanvasImage *_canvasImage;
}

- (instancetype)initWithCanvasImage:(AtomCanvasImage *)pCanvasImage {
    self = [super init];
    if (self) {
        _canvasImage = pCanvasImage;
    }
    return self;
}

- (AtomCanvasImage *)getCanvasImage {
    return _canvasImage;
}


@end

@implementation CanvasFillStyleJavaScriptInterface {
    AtomCanvasFillStyle *_canvasFillStyle;
}

- (instancetype)initWithAtomCanvasFillStyle:(AtomCanvasFillStyle *)pAtomCanvasFillStyle {
    self = [super init];
    if (self) {
        _canvasFillStyle = pAtomCanvasFillStyle;
    }
    return self;
}

- (AtomCanvasFillStyle *)getCanvasFillStyle {
    return _canvasFillStyle;
}

- (void)addColorStop:(float)stop :(NSString *)colorStr {
    if (_canvasFillStyle->isLinearGradient()) {
        AtomCanvasLinearGradient *linearGradient = (AtomCanvasLinearGradient *) _canvasFillStyle;
        linearGradient->addColorStop(stop, std::string([colorStr UTF8String]));
    } else if (_canvasFillStyle->isRadialGradient()) {
        AtomCanvasRadialGradient *radialGradient = (AtomCanvasRadialGradient *) _canvasFillStyle;
        radialGradient->addColorStop(stop, std::string([colorStr UTF8String]));
    }
}

- (CanvasFillStyle)getCanvasFillStyleEnum {
    return (CanvasFillStyle) _canvasFillStyle->getStyle();
}
@end


@implementation CanvasContextJavaScriptInterface {
    CanvasContext2D *_canvasContext2d;
}

- (instancetype)initWithCanvasContext:(CanvasContext2D *)pCanvasContext2d {
    self = [super init];
    if (self) {
        _canvasContext2d = pCanvasContext2d;
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
    Color4F colorStyle = StrValueToColor4F([shadowColor cString]);
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

- (void)setFillStyle:(id)fillStyle {
    if ([fillStyle isKindOfClass:[NSString class]]) {
        Color4F colorStyle = StrValueToColor4F([fillStyle cString]);
        _canvasContext2d->setFillStyle(colorStyle);
    } else if ([fillStyle isKindOfClass:[CanvasFillStyleJavaScriptInterface class]]) {
        CanvasFillStyleJavaScriptInterface *anInterface = (CanvasFillStyleJavaScriptInterface *) fillStyle;
        switch ([anInterface getCanvasFillStyleEnum]) {
            case CanvasFillStyleLinearGradient: {
                AtomGraphics::AtomCanvasLinearGradient *gradient = (AtomGraphics::AtomCanvasLinearGradient *) [anInterface getCanvasFillStyle];
                _canvasContext2d->setFillStyle(gradient);
            }
                break;
            case CanvasFillStyleRadialGradient: {
                AtomGraphics::AtomCanvasRadialGradient *gradient = (AtomGraphics::AtomCanvasRadialGradient *) [anInterface getCanvasFillStyle];
                _canvasContext2d->setFillStyle(gradient);
            }
                break;
            case CanvasFillStylePattern: {
                AtomGraphics::AtomCanvasPattern *pattern = (AtomGraphics::AtomCanvasPattern *) [anInterface getCanvasFillStyle];
                _canvasContext2d->setFillStyle(pattern);
            }
                break;
            default:
                break;
        }
    }
}

- (id)strokeStyle {
    return @"";
}

- (void)setStrokeStyle:(id)strokeStyle {
    if ([strokeStyle isKindOfClass:[NSString class]]) {
        Color4F colorStyle = StrValueToColor4F([strokeStyle cString]);
        _canvasContext2d->setStrokeStyle(colorStyle);
    } else {// undefined
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
        _canvasContext2d->arc(floatByPrecision(x, 2), floatByPrecision(y, 2), floatByPrecision(r, 2),
                floatByPrecision(sAngle, 2), floatByPrecision(eAngle, 2), counterclockwise);
    }
}


- (void)stroke {
    _canvasContext2d->stroke();
}

- (void)fill {
    _canvasContext2d->fill();
}

- (void)save {
    _canvasContext2d->save();
}

- (void)restore {
    _canvasContext2d->restore();
}

- (void)moveTo:(double)x :(double)y {
    _canvasContext2d->moveTo(floatByPrecision(x, 2), floatByPrecision(y, 2));
}

- (void)lineTo:(double)x :(double)y {
    _canvasContext2d->lineTo(floatByPrecision(x, 2), floatByPrecision(y, 2));
}

- (double)lineWidth {
    return 0;
}

- (void)setLineWidth:(double)lineWidth {
    _canvasContext2d->setLineWidth(floatByPrecision(lineWidth, 1));
}

- (NSString *)lineCap {
    return @"";
}

- (void)setLineCap:(NSString *)lineCap {
    _canvasContext2d->setLineCap(std::string([lineCap UTF8String]));
}

- (NSString *)lineJoin {
    return @"";
}

- (void)setLineJoin:(NSString *)lineJoin {
    _canvasContext2d->setLineJoin(std::string([lineJoin UTF8String]));
}

- (float)miterLimit {
    return 0;
}

- (void)setMiterLimit:(float)miterLimit {
    _canvasContext2d->setMiterLimit(miterLimit);
}

- (void)rect:(float)x :(float)y :(float)width :(float)height {
    if (width && height) {
        _canvasContext2d->setRect(x, y, width, height);
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
    _canvasContext2d->rotate(angle);
}

- (void)translate:(float)x :(float)y {
    _canvasContext2d->translate(x, y);
}

- (void)transform:(float)a :(float)b :(float)c :(float)d :(float)e :(float)f {
    _canvasContext2d->transform(a, b, c, d, e, f);
}

- (void)setTransform:(double)a :(double)b :(double)c :(double)d :(double)e :(double)f {
    _canvasContext2d->setTransform(floatByPrecision(a, 2), floatByPrecision(b, 2), floatByPrecision(c, 2),
            floatByPrecision(d, 2), floatByPrecision(e, 2), floatByPrecision(f, 2));
}

- (void)fillText {
    NSArray *args = [JSContext currentArguments];
    int argCount = [args count];
    if (argCount >= 3) {
        NSString *text = [args[0] toString];
        double x = [args[1] toDouble];
        double y = [args[2] toDouble];
        float maxWidth = argCount >= 4 ? (float) [args[3] toDouble] : 0.f;
        _canvasContext2d->fillText(std::string([text cStringUsingEncoding:NSUTF8StringEncoding]), floatByPrecision(x, 2), floatByPrecision(y, 2), maxWidth);
    } else {
        NSLog(@"args are not valid");
    }
}

- (NSString *)font {
    return @"";
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
    _canvasContext2d->setTextAlign(std::string([textAlign UTF8String]));
}

- (NSString *)textBaseline {
    return @"";
}

- (void)setTextBaseline:(NSString *)textBaseline {
    _canvasContext2d->setTextBaseline(std::string([textBaseline UTF8String]));
}

- (void)strokeText {
    NSArray *args = [JSContext currentArguments];
    int argCount = [args count];
    if (argCount >= 3) {
        NSString *text = [args[0] toString];
        double x = [args[1] toDouble];
        double y = [args[2] toDouble];
        float maxWidth = argCount >= 4 ? (float) [args[3] toDouble] : 0.f;
        _canvasContext2d->strokeText(std::string([text cString]), x, y, maxWidth);
    } else {
        NSLog(@"args are not valid");
    }
}

- (double)globalAlpha {
    return 0;
}

- (void)setGlobalAlpha:(double)globalAlpha {
    _canvasContext2d->setGlobalAlpha(floatByPrecision(globalAlpha, 1));
}

- (NSString *)globalCompositeOperation {
    return @"";
}

- (void)setGlobalCompositeOperation:(NSString *)globalCompositeOperation {
    _canvasContext2d->setGlobalCompositeOperation(std::string([globalCompositeOperation UTF8String]));
}

- (void)bezierCurveTo:(float)cp1x :(float)cp1y :(float)cp2x :(float)cp2y :(float)x :(float)y {
    _canvasContext2d->bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

- (CanvasFillStyleJavaScriptInterface *)createPattern:(CanvasImageJavaScriptInterface *)image :(NSString *)pattern {
    //todo 这里的image还没有开放对外声明的一个API定义（还没有明确的语法需求），所以先写死一个src，实现这个效果
    AtomCanvasImage *canvasImage = new AtomCanvasImage();
    std::string imageSrc = "http://www.iconpng.com/png/miui-love/browser.png";
    canvasImage->setSrc(imageSrc);
    AtomCanvasPattern *canvasPattern = _canvasContext2d->createPattern(std::string([pattern cString]), canvasImage);

//    [[AtomGraphicsGCanvasModule sharedModule] preLoadImage:canvasImage->getSrc() imageId:canvasImage->getId() :^(BOOL succ, id result) {
//        /**
//         * todo 目前，因为指令顺序执行，过程中缺少同步资源的一个状态
//         * 可能导致在fill的时刻 图片资源是空的，最后绘制空场景
//         *
//         * 1.图表针对移动端这套方案，在所有指令之前，需要先确保资源的预加载和绑定
//         *
//         * 2.如果不能做到统一的预加载，还有一种选择是在fill的时候判断是否是image类型的pattern
//         */
//        if (!succ) {
//            NSLog(@"fail to preload image");
//        }
//    }];
    CanvasFillStyleJavaScriptInterface *anInterface = [[CanvasFillStyleJavaScriptInterface alloc] initWithAtomCanvasFillStyle:canvasPattern];
    return anInterface;
}

- (CanvasFillStyleJavaScriptInterface *)createLinearGradient:(float)x0 :(float)y0 :(float)x1 :(float)y1 {
    AtomCanvasLinearGradient *atomLinearGradient = _canvasContext2d->createLinearGradient(x0, y0, x1, y1);
    CanvasFillStyleJavaScriptInterface *anInterface = [[CanvasFillStyleJavaScriptInterface alloc] initWithAtomCanvasFillStyle:atomLinearGradient];
    return anInterface;
}

- (CanvasFillStyleJavaScriptInterface *)createRadialGradient:(float)x0 :(float)y0 :(float)r0 :(float)x1 :(float)y1 :(float)r1 {
    AtomCanvasRadialGradient *atomRadialGradient = _canvasContext2d->createRadialGradient(x0, y0, r0, x1, y1, r1);
    CanvasFillStyleJavaScriptInterface *anInterface = [[CanvasFillStyleJavaScriptInterface alloc] initWithAtomCanvasFillStyle:atomRadialGradient];
    return anInterface;
}

- (CanvasImageJavaScriptInterface *)getImageData:(int)x :(int)y :(int)width :(int)height {
    _canvasContext2d->getImageData(x, y, width, height);
    return nullptr;
}

- (void)drawImage {
    NSArray *args = [JSContext currentArguments];
    NSInteger argCount = [args count];

    if (argCount > 0 && [NSStringFromClass([[args[0] toObject] class]) isEqualToString:@"CanvasJavaScriptInterface"]) {
        //draw canvas dom type
        CanvasJavaScriptInterface *canvasDom = (CanvasJavaScriptInterface *) [args[0] toObject];
        CanvasNode *node = [canvasDom getCanvasNode];
        _canvasContext2d->drawImage(node);
    } else {
        //todo 确认image对象 并转换 接口待定
//        AtomCanvasImage *canvasImage = new AtomCanvasImage();
//        std::string imageSrc = "https://gw.alicdn.com/tfs/TB1KwRTlh6I8KJjy0FgXXXXzVXa-225-75.png";
//        canvasImage->setSrc(imageSrc);
//
//        if (argCount == 3) {
//            float x = (float) [args[1] toDouble];
//            float y = (float) [args[2] toDouble];
//            [[AtomGraphicsGCanvasModule sharedModule] preLoadImage:canvasImage->getSrc() imageId:canvasImage->getId() :^(BOOL succ, id result) {
//                if (succ) {
//                    canvasImage->setWidth([result[@"width"] floatValue]);
//                    canvasImage->setHeight([result[@"height"] floatValue]);
//                    _canvasContext2d->drawImage(canvasImage, x, y);
//                } else {
//                    NSLog(@"fail to preload image");
//                }
//            }];
//        } else if (argCount == 5) {
//            float x = (float) [args[1] toDouble];
//            float y = (float) [args[2] toDouble];
//            float width = (float) [args[3] toDouble];
//            float height = (float) [args[4] toDouble];
//            [[AtomGraphicsGCanvasModule sharedModule] preLoadImage:canvasImage->getSrc() imageId:canvasImage->getId() :^(BOOL succ, id result) {
//                if (succ) {
//                    canvasImage->setWidth([result[@"width"] floatValue]);
//                    canvasImage->setHeight([result[@"height"] floatValue]);
//                    _canvasContext2d->drawImage(canvasImage, x, y, width, height);
//                } else {
//                    NSLog(@"fail to preload image");
//                }
//            }];
//        } else if (argCount == 9) {
//            float sx = (float) [args[1] toDouble];
//            float sy = (float) [args[2] toDouble];
//            float swidth = (float) [args[3] toDouble];
//            float sheight = (float) [args[4] toDouble];
//            float x = (float) [args[5] toDouble];
//            float y = (float) [args[6] toDouble];
//            float width = (float) [args[7] toDouble];
//            float height = (float) [args[8] toDouble];
//            [[AtomGraphicsGCanvasModule sharedModule] preLoadImage:canvasImage->getSrc() imageId:canvasImage->getId() :^(BOOL succ, id result) {
//                if (succ) {
//                    canvasImage->setWidth([result[@"width"] floatValue]);
//                    canvasImage->setHeight([result[@"height"] floatValue]);
//                    _canvasContext2d->drawImage(canvasImage, sx, sy, swidth, sheight, x, y, width, height);
//                } else {
//                    NSLog(@"fail to preload image");
//                }
//            }];
//        }
    }
}

- (JSValue *)createImageData {
    NSArray *args = [JSContext currentArguments];
    NSMutableArray *callArgs = [NSMutableArray array];
    if ([args count] == 1) {
        NSDictionary *argDict = [(JSValue *) args[0] toDictionary];
        NSNumber *width = argDict[@"width"];
        NSNumber *height = argDict[@"height"];
        if (width && height) {
            [callArgs addObject:width];
            [callArgs addObject:height];
        }
    } else if ([args count] == 2) {
        [callArgs addObject:args[0]];
        [callArgs addObject:args[1]];
    }
    JSValue *jsFunc = [[JSContext currentContext] evaluateScript:@"_contextCreateImageData"];
    return [jsFunc callWithArguments:callArgs];
}

- (void)putImageData {
    NSArray *args = [JSContext currentArguments];
    int argCount = [args count];
    if (argCount == 3) { //imageData ,x , y
        JSValue *imageData = args[0];
        int x = [args[1] toInt32];
        int y = [args[2] toInt32];

        NSDictionary *imageDataDict = [imageData toDictionary];
        if (imageDataDict) {
            int width = [imageDataDict[@"width"] intValue];
            int height = [imageDataDict[@"height"] intValue];
            NSArray *dataArr = imageDataDict[@"data"];
            AtomCanvasImageData *canvasImageData = new AtomCanvasImageData(width, height);
            int *data = new int [[dataArr count]];

            for (int i = 0; i < [dataArr count]; i += 4) {
                unsigned int index = (unsigned int) i + 0;
                data[index] = [dataArr[index] intValue];
                data[index + 1] = [dataArr[index + 1] intValue];
                data[index + 2] = [dataArr[index + 2] intValue];
                data[index + 3] = [dataArr[index + 3] intValue];
            }
            canvasImageData->setData(data);
            _canvasContext2d->putImageData(canvasImageData, x, y, width, height, width, height);
        }
    } else if (argCount > 3 && argCount < 8) { //imageData, x,y ,and optional dirtyX,dirtyY,dirtyWidth,dirtyHeight
        JSValue *imageData = args[0];
        int x = [args[1] toInt32];
        int y = [args[2] toInt32];
        int dirtyX = argCount > 3 ? [args[3] toInt32] : 0;
        int dirtyY = argCount > 4 ? [args[4] toInt32] : 0;
        int dirtyWidth = argCount > 5 ? [args[5] toInt32] : 0;
        int dirtyHeight = argCount > 6 ? [args[6] toInt32] : 0;

        NSDictionary *imageDataDict = [imageData toDictionary];

        if (imageDataDict) {
            int width = [imageDataDict[@"width"] intValue];
            int height = [imageDataDict[@"height"] intValue];
            NSArray *dataArr = imageDataDict[@"data"];


            int restWidth = 0, restHeight = 0;
            if (dirtyX >= 0 && dirtyX < width && dirtyY >= 0 && dirtyY < height) {
                restWidth = width - dirtyX;
                restHeight = height - dirtyY;
            }
            if (restWidth > 0 && restHeight > 0) {
                if (dirtyWidth > restWidth) {
                    dirtyWidth = restWidth;
                }
                if (dirtyHeight > restHeight) {
                    dirtyHeight = restHeight;
                }

                AtomCanvasImageData *canvasImageData = new AtomCanvasImageData(dirtyWidth, dirtyHeight);
                int dirtyDataCount = 4 * dirtyWidth * dirtyHeight;
                int *data = new int[dirtyDataCount];

                for (int j = dirtyY, l = 0; j < dirtyY + dirtyHeight; j++, l += 4) {
                    for (int i = dirtyX, k = 0; i < dirtyX + dirtyWidth; i++, k += 4) {
                        //目标数据块对应的坐标（dirtyData）
                        unsigned int index = (unsigned int) (l * dirtyWidth + k);
                        //源数据块对应的坐标（较大的数据画布）
                        unsigned int dirtyIndex = (unsigned int) (j * width + i) * 4;
                        //一个数据点有4个数据位
                        data[index] = [dataArr[dirtyIndex] intValue];
                        data[index + 1] = [dataArr[dirtyIndex + 1] intValue];
                        data[index + 2] = [dataArr[dirtyIndex + 2] intValue];
                        data[index + 3] = [dataArr[dirtyIndex + 3] intValue];
                    }
                }
                canvasImageData->setData(data);
                _canvasContext2d->putImageData(canvasImageData, x, y, dirtyWidth, dirtyWidth, dirtyWidth, dirtyHeight);
            }
        }
    }
}

- (JSValue *)measureText {
    NSArray *args = [JSContext currentArguments];
    float width = 0.f;
    if ([args count]) {
        try {
            JSValue *textValue = args[0];
            width = _canvasContext2d->measureText([[textValue toString] cStringUsingEncoding:NSUTF8StringEncoding]);
        } catch (NSException *exception) {
            NSLog(@"%@", exception);
            width = 0;
        }
    }
    JSValue *value = [JSValue valueWithObject:@{@"width": @(width)} inContext:[JSContext currentContext]];
    return value;
}
@end