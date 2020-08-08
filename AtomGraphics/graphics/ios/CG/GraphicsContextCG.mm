//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsContextCG.h"
#import "ColorCG.h"
#import "GraphicsContextStateSaver.h"
#import "CoreGraphicsAPIExtension.h"
#import "CGContextStateSaver.h"
#import "ImageBuffer.h"
#import "AtomLog.h"


namespace AtomGraphics {

inline CGAffineTransform getUserToBaseCTM(CGContextRef context) {
//    return CGAffineTransformConcat(CGContextGetCTM(context), CGAffineTransformInvert(CGContextGetBaseCTM(context)));
    return CGContextGetCTM(context);
}

GraphicsContextCG::GraphicsContextCG(RetainPtr<CGContextRef> platformContext)
        : m_context(std::move(platformContext)) {

}

GraphicsContextCG::~GraphicsContextCG() {}

void GraphicsContextCG::setFillStyle(const CanvasStyle &style) {

    if (style.isColorStyle()) {
        m_state.fillColor = style.getColor();
        m_state.fillGradient = nullptr;
        m_state.fillPattern = nullptr;
        CGContextSetFillColorWithColor(m_context.get(), cachedCGColor(style.getColor()));
    } else if (style.isGradientStyle()) {
        m_state.fillGradient = style.getCanvasGradient();
        m_state.fillPattern = nullptr;
    } else if (style.isPatternStyle()) {
        m_state.fillGradient = nullptr;
        m_state.fillPattern = style.getCanvasPattern();
    }
}

void GraphicsContextCG::setStrokeStyle(const CanvasStyle &style) {

    if (style.isColorStyle()) {
        m_state.strokeColor = style.getColor();
        m_state.strokeGradient = nullptr;
        m_state.strokePattern = nullptr;
        CGContextSetStrokeColorWithColor(m_context.get(), cachedCGColor(style.getColor()));
    } else if (style.isGradientStyle()) {
        m_state.strokeGradient = style.getCanvasGradient();
        m_state.strokePattern = nullptr;
    } else if (style.isPatternStyle()) {
        m_state.strokeColor = style.getColor();
        m_state.strokeGradient = nullptr;
        m_state.strokePattern = style.getCanvasPattern();
    }
}

void GraphicsContextCG::setShadow(const FloatSize &offset, float blurRadius, const Color4F &shadowColor) {
    blurRadius = std::min(blurRadius, 1000.0f);
    CGContextSetShadowWithColor(m_context.get(), offset, blurRadius, cachedCGColor(shadowColor));
}

void GraphicsContextCG::setLineCap(const LineCap lineCap) {

    CGLineCap lineCapValueCG;
    switch (lineCap) {
        case LineCap::RoundCap:
            lineCapValueCG = kCGLineCapRound;
            break;
        case LineCap::ButtCap:
            lineCapValueCG = kCGLineCapButt;
            break;
        case LineCap::SquareCap:
            lineCapValueCG = kCGLineCapSquare;
            break;
        default:
            return;
    }

    CGContextSetLineCap(m_context.get(), lineCapValueCG);
}

void GraphicsContextCG::setLineJoin(const LineJoin lineJoin) {

    CGLineJoin lineJoinValueCG;
    switch (lineJoin) {
        case LineJoin::RoundJoin:
            lineJoinValueCG = kCGLineJoinRound;
            break;
        case LineJoin::BevelJoin:
            lineJoinValueCG = kCGLineJoinBevel;
            break;
        case LineJoin::MiterJoin:
            lineJoinValueCG = kCGLineJoinMiter;
            break;
        default: {
            return;
        }
    }

    CGContextSetLineJoin(m_context.get(), lineJoinValueCG);
}

void GraphicsContextCG::setStrokeThickness(float thickness) {
    m_state.strokeThickness = thickness;
    CGContextSetLineWidth(m_context.get(), std::max(thickness, 0.f));
}

void GraphicsContextCG::setLineDash(const DashArray &dashes, float dashOffset) {

    if (paintingDisabled())
        return;

    if (dashOffset < 0) {
        float length = 0;
        for (size_t i = 0; i < dashes.size(); ++i)
            length += static_cast<float>(dashes[i]);
        if (length)
            dashOffset = fmod(dashOffset, length) + length;
    }

    CGContextSetLineDash(m_context.get(), dashOffset, dashes.data(), dashes.size());
}

void GraphicsContextCG::setMiterLimit(float limit) {
    if (paintingDisabled()) {
        return;
    }

    CGContextSetMiterLimit(m_context.get(), std::max(limit, 0.f));
}

void GraphicsContextCG::fillRect(const FloatRect &rect) {
    if (paintingDisabled())
        return;

    if (m_state.fillGradient) {
        GraphicsContextStateSaver stateSaver(this);
        if (hasShadow()) {
            FloatSize layerSize = getTransform().mapSize(FloatSize(rect.size));

            CGLayerRef layer = CGLayerCreateWithContext(m_context.get(), layerSize, 0);
            CGContextRef layerContext = CGLayerGetContext(layer);

            CGContextScaleCTM(layerContext, layerSize.width / rect.size.width, layerSize.height / rect.size.height);
            CGContextTranslateCTM(layerContext, -rect.origin.x, -rect.origin.y);
            CGContextAddRect(layerContext, rect);
            CGContextClip(layerContext);

            m_state.fillGradient->paint(layerContext);
            CGContextDrawLayerInRect(m_context.get(), rect, layer);
            CGLayerRelease(layer);
        } else {
            CGContextClipToRect(m_context.get(), rect);
            m_state.fillGradient->paint(m_context.get());
        }
        return;
    }

    if (m_state.fillPattern)
        applyFillPattern();

    CGContextFillRect(m_context.get(), rect);
}

void GraphicsContextCG::strokeRect(const FloatRect &rect, float lineWidth) {

    if (m_state.strokeGradient) {
        float lineWidth = m_state.strokeThickness;
        if (hasShadow()) {
            const float doubleLineWidth = lineWidth * 2;
            float adjustedWidth = ceilf(rect.size.width + doubleLineWidth);
            float adjustedHeight = ceilf(rect.size.height + doubleLineWidth);
            FloatSize layerSize = getTransform().mapSize(FloatSize(adjustedWidth, adjustedHeight));

            CGLayerRef layer = CGLayerCreateWithContext(m_context.get(), layerSize, 0);

            CGContextRef layerContext = CGLayerGetContext(layer);
            m_state.strokeThickness = lineWidth;
            CGContextSetLineWidth(layerContext, lineWidth);

            const float translationX = lineWidth - rect.origin.x;
            const float translationY = lineWidth - rect.origin.y;
            CGContextScaleCTM(layerContext, layerSize.width / adjustedWidth, layerSize.height / adjustedHeight);
            CGContextTranslateCTM(layerContext, translationX, translationY);

            CGContextAddRect(layerContext, rect);
            CGContextReplacePathWithStrokedPath(layerContext);
            CGContextClip(layerContext);
            m_state.strokeGradient->paint(layerContext);

            const float destinationX = roundf(rect.origin.x - lineWidth);
            const float destinationY = roundf(rect.origin.y - lineWidth);
            CGContextDrawLayerInRect(m_context.get(), CGRectMake(destinationX, destinationY, adjustedWidth, adjustedHeight), layer);
            CGLayerRelease(layer);
        } else {
            CGContextStateSaver stateSaver(m_context.get());
            CGContextAddRect(m_context.get(), rect);
            CGContextReplacePathWithStrokedPath(m_context.get());
            CGContextClip(m_context.get());
            m_state.strokeGradient->paint(m_context.get());
        }

        return;
    }

    if (m_state.strokePattern)
        applyStrokePattern();

    GraphicsContextStateSaver stateSaver(this);

    CGContextAddRect(m_context.get(), rect);
    CGContextStrokePath(m_context.get());
}

void GraphicsContextCG::applyStrokePattern() {
    if (!m_context.get())
        return;

    AffineTransform userToBaseCTM = AffineTransform(getUserToBaseCTM(m_context.get()));

    CGPatternRef platformPattern = m_state.strokePattern->createPlatformPattern(userToBaseCTM);
    if (!platformPattern)
        return;

    CGColorSpaceRef patternSpace = CGColorSpaceCreatePattern(0);
    CGContextSetStrokeColorSpace(m_context.get(), patternSpace);

    const CGFloat patternAlpha = 1;
    CGContextSetStrokePattern(m_context.get(), platformPattern, &patternAlpha);
}

void GraphicsContextCG::applyFillPattern() {
    if (!m_context.get())
        return;

    AffineTransform userToBaseCTM = AffineTransform(getUserToBaseCTM(m_context.get()));

    CGPatternRef platformPattern = m_state.fillPattern->createPlatformPattern(userToBaseCTM);
    if (!platformPattern)
        return;

    CGColorSpaceRef patternSpace = CGColorSpaceCreatePattern(nullptr);
    CGContextSetFillColorSpace(m_context.get(), patternSpace);

    const CGFloat patternAlpha = 1;
    CGContextSetFillPattern(m_context.get(), platformPattern, &patternAlpha);
}

void GraphicsContextCG::clearRect(const FloatRect &rect) {
    CGContextClearRect(m_context.get(), rect);
}

void GraphicsContextCG::fillPath(const Path &path) {

    if (!m_context || path.isEmpty())
        return;

    if (m_state.fillGradient) {
        if (hasShadow()) {
            FloatRect rect = path.boundingRect();
            FloatSize layerSize = getTransform().mapSize(IntSize(rect.size));

            CGLayerRef layer = CGLayerCreateWithContext(m_context.get(), layerSize, 0);
            CGContextRef layerContext = CGLayerGetContext(layer);

            CGContextScaleCTM(layerContext, layerSize.width / rect.size.width, layerSize.height / rect.size.height);
            CGContextTranslateCTM(layerContext, -rect.origin.x, -rect.origin.y);
            CGContextBeginPath(layerContext);
            CGContextAddPath(layerContext, path.getCGPath());

            if (fillRule() == WindRule::EvenOdd)
                CGContextEOClip(layerContext);
            else
                CGContextClip(layerContext);

            m_state.fillGradient->paint(layerContext);
            CGContextDrawLayerInRect(m_context.get(), rect, layer);
            CGLayerRelease(layer);
        } else {
            CGContextBeginPath(m_context.get());
            CGContextAddPath(m_context.get(), path.getCGPath());
            CGContextStateSaver stateSaver(m_context.get());

            if (fillRule() == WindRule::EvenOdd)
                CGContextEOClip(m_context.get());
            else
                CGContextClip(m_context.get());

            m_state.fillGradient->paint(m_context.get());
        }

        return;
    }

    if (m_state.fillPattern)
        applyFillPattern();

    ContextDrawPath(m_context.get(), fillRule() == WindRule::EvenOdd ? kCGPathEOFill : kCGPathFill, path.getCGPath(), nullptr);
}

void GraphicsContextCG::strokePath(Path *path) {
    if (!m_context.get()) {
        return;
    }

    if (!path) {
        return;
    }

    if (m_state.strokeGradient) {
        if (hasShadow()) {
            FloatRect rect = path->boundingRect();
            float lineWidth = m_state.strokeThickness;
            float doubleLineWidth = lineWidth * 2;
            float adjustedWidth = ceilf(rect.size.width + doubleLineWidth);
            float adjustedHeight = ceilf(rect.size.height + doubleLineWidth);

            FloatSize layerSize = getTransform().mapSize(FloatSize(adjustedWidth, adjustedHeight));

            CGLayerRef layer = CGLayerCreateWithContext(m_context.get(), layerSize, 0);
            CGContextRef layerContext = CGLayerGetContext(layer);
            CGContextSetLineWidth(layerContext, lineWidth);

            float translationX = lineWidth - rect.origin.x;
            float translationY = lineWidth - rect.origin.y;
            CGContextScaleCTM(layerContext, layerSize.width / adjustedWidth, layerSize.height / adjustedHeight);
            CGContextTranslateCTM(layerContext, translationX, translationY);

            CGContextAddPath(layerContext, path->getCGPath());
            CGContextReplacePathWithStrokedPath(layerContext);
            CGContextClip(layerContext);
            m_state.strokeGradient->paint(layerContext);

            float destinationX = roundf(rect.origin.x - lineWidth);
            float destinationY = roundf(rect.origin.y - lineWidth);
            CGContextDrawLayerInRect(m_context.get(), CGRectMake(destinationX, destinationY, adjustedWidth, adjustedHeight), layer);
            CGLayerRelease(layer);
        } else {
            CGContextStateSaver stateSaver(m_context.get());
            CGContextBeginPath(m_context.get());
            CGContextAddPath(m_context.get(), path->getCGPath());
            CGContextReplacePathWithStrokedPath(m_context.get());
            CGContextClip(m_context.get());
            m_state.strokeGradient->paint(m_context.get());
        }
        return;
    }

    if (m_state.strokePattern)
        applyStrokePattern();

    ContextDrawPath(m_context.get(), kCGPathStroke, path->getCGPath(), nullptr);
}

void GraphicsContextCG::clipPath(Path *path, WindRule clipRule) {
    if (!m_context.get()) {
        return;
    }

    if (!path) {
        return;
    }

    if (path->isEmpty())
        CGContextClipToRect(m_context.get(), CGRectZero);
    else {
        CGContextBeginPath(m_context.get());
        CGContextAddPath(m_context.get(), path->getCGPath());

        if (clipRule == WindRule::EvenOdd)
            CGContextEOClip(m_context.get());
        else
            CGContextClip(m_context.get());
    }
}

void GraphicsContextCG::clipRect(const FloatRect &rect) {
    if (paintingDisabled())
        return;

    CGContextClipToRect(m_context.get(), rect);
}

void GraphicsContextCG::clipOut(const FloatRect &rect) {
    const AffineTransform &ctm = getTransform();
    CGRect rects[2] = {CGRectInfinite, rect};
    CGContextBeginPath(getCGContextRef());
    CGContextAddRects(getCGContextRef(), rects, 2);
    CGContextEOClip(getCGContextRef());
}

void GraphicsContextCG::clipToImageBuffer(ImageBuffer &buffer, const FloatRect &destRect) {
    if (!m_context.get())
        return;

    FloatSize bufferDestinationSize = buffer.sizeForDestinationSize(destRect.size);
    scoped_refptr<Image> image = buffer.copyNativeImage(BackingStoreCopy::DontCopyBackingStore);

    // FIXME: This image needs to be grayscale to be used as an alpha mask here.
    CGContextTranslateCTM(m_context.get(), destRect.x(), destRect.y() + bufferDestinationSize.height);
    CGContextScaleCTM(m_context.get(), 1, -1);
    CGContextClipToRect(m_context.get(), FloatRect(FloatPoint(0, bufferDestinationSize.height - destRect.height()), destRect.size));
    CGContextClipToMask(m_context.get(), FloatRect(FloatPoint(), bufferDestinationSize), image.get()->getCGImage());
    CGContextScaleCTM(m_context.get(), 1, -1);
    CGContextTranslateCTM(m_context.get(), -destRect.x(), -destRect.y() - destRect.height());
}


void GraphicsContextCG::scale(float sx, float sy) {
    if (paintingDisabled())
        return;

    CGContextScaleCTM(m_context.get(), sx, sy);
}

void GraphicsContextCG::rotate(float angleInRadians) {

    if (paintingDisabled())
        return;

    CGContextRotateCTM(m_context.get(), angleInRadians);
}

void GraphicsContextCG::translate(float x, float y) {
    if (paintingDisabled())
        return;

    CGContextTranslateCTM(m_context.get(), x, y);
}

void GraphicsContextCG::concat(const AffineTransform &ctm) {
    if (paintingDisabled())
        return;

    CGContextConcatCTM(m_context.get(), ctm);
}

AffineTransform GraphicsContextCG::getTransform() {
    if (!m_context.get()) {
        return AffineTransform();
    }

    return CGContextGetCTM(m_context.get());
}

void GraphicsContextCG::setTransform(const AffineTransform &transform) {
    if (paintingDisabled())
        return;

    ContextSetCTM(m_context.get(), transform);
}

void GraphicsContextCG::drawImage(Image *image, const FloatRect &destRect, const FloatRect &srcRect, CompositeOperator op, BlendMode blendMode) {
    if (!image || !m_context) {
        return;
    }

    CGImageRef subImage = image->getCGImage();
    IntSize imageSize = image->size();

    GraphicsContextStateSaver stateSaver(this);
    bool shouldUseSubimage = false;
    FloatRect adjustedDestRect = destRect;

    if (srcRect.size != image->size()) {
        CGInterpolationQuality interpolationQuality = CGContextGetInterpolationQuality(m_context.get());
        // When the image is scaled using high-quality interpolation, we create a temporary CGImage
        // containing only the portion we want to display. We need to do this because high-quality
        // interpolation smoothes sharp edges, causing pixels from outside the source rect to bleed
        // into the destination rect. See <rdar://problem/6112909>.
        shouldUseSubimage = (interpolationQuality != kCGInterpolationNone) && (srcRect.size != destRect.size || !getTransform().isIdentityOrTranslationOrFlipped());
        float xScale = srcRect.width() / destRect.width();
        float yScale = srcRect.height() / destRect.height();
        if (shouldUseSubimage) {
            FloatRect subimageRect = srcRect;
            float leftPadding = srcRect.x() - floorf(srcRect.x());
            float topPadding = srcRect.y() - floorf(srcRect.y());

            subimageRect.move(-leftPadding, -topPadding);
            adjustedDestRect.move(-leftPadding / xScale, -topPadding / yScale);

            subimageRect.setWidth(ceilf(subimageRect.width() + leftPadding));
            adjustedDestRect.setWidth(subimageRect.width() / xScale);

            subimageRect.setHeight(ceilf(subimageRect.height() + topPadding));
            adjustedDestRect.setHeight(subimageRect.height() / yScale);

            subImage = CGImageCreateWithImageInRect(subImage, subimageRect);

            if (image->height() < srcRect.getMaxY()) {
                adjustedDestRect.setHeight(CGImageGetHeight(subImage) / yScale);
            }
        } else {
            adjustedDestRect.origin = FloatPoint(destRect.x() - srcRect.x() / xScale, destRect.y() - srcRect.y() / yScale);
            adjustedDestRect.size = FloatSize(imageSize.width / xScale, imageSize.height / yScale);
        }

        if (!destRect.containsRect(adjustedDestRect)) {
            CGContextClipToRect(m_context.get(), destRect);
        }
    }

    // If the image is only partially loaded, then shrink the destination rect that we're drawing into accordingly.
    if (!shouldUseSubimage && image->height() < imageSize.height)
        adjustedDestRect.setHeight(adjustedDestRect.height() * image->height() / imageSize.height);

    // Anti-aliasing is on by default on the iPhone. Need to turn it off when drawing images.
    CGContextSetShouldAntialias(m_context.get(), false);

    // Align to pixel boundaries
    adjustedDestRect = roundToDevicePixels(adjustedDestRect);

    setCompositeOperation(op, blendMode);

    // ImageOrientation expects the origin to be at (0, 0)
    CGContextTranslateCTM(m_context.get(), adjustedDestRect.x(), adjustedDestRect.y());
    adjustedDestRect.origin = FloatPoint();

    // Flip the coords.
    CGContextTranslateCTM(m_context.get(), 0, adjustedDestRect.height());
    CGContextScaleCTM(m_context.get(), 1, -1);

    // Draw the image.
    CGContextDrawImage(m_context.get(), adjustedDestRect, subImage);

    if (shouldUseSubimage) {
        CGImageRelease(subImage);
    }
}

void GraphicsContextCG::drawText(const base::String &text, float x, float y, const Font &font, bool fill) {
    CGContextStateSaver saver(m_context.get());
    if (fill) {
        CGContextSetTextDrawingMode(m_context.get(), kCGTextFill);
    } else {
        CGContextSetTextDrawingMode(m_context.get(), kCGTextStroke);
    }
    font.drawBidiText(m_context.get(), text, {x, y});
}

void GraphicsContextCG::setGlobalAlpha(float alpha) {
    CGContextSetAlpha(m_context.get(), alpha);
}

static CGBlendMode selectCGBlendMode(CompositeOperator compositeOperator, BlendMode blendMode) {
    switch (blendMode) {
        case BlendMode::Normal:
            switch (compositeOperator) {
                case CompositeClear:
                    return kCGBlendModeClear;
                case CompositeCopy:
                    return kCGBlendModeCopy;
                case CompositeSourceOver:
                    return kCGBlendModeNormal;
                case CompositeSourceIn:
                    return kCGBlendModeSourceIn;
                case CompositeSourceOut:
                    return kCGBlendModeSourceOut;
                case CompositeSourceAtop:
                    return kCGBlendModeSourceAtop;
                case CompositeDestinationOver:
                    return kCGBlendModeDestinationOver;
                case CompositeDestinationIn:
                    return kCGBlendModeDestinationIn;
                case CompositeDestinationOut:
                    return kCGBlendModeDestinationOut;
                case CompositeDestinationAtop:
                    return kCGBlendModeDestinationAtop;
                case CompositeXOR:
                    return kCGBlendModeXOR;
                case CompositePlusDarker:
                    return kCGBlendModePlusDarker;
                case CompositePlusLighter:
                    return kCGBlendModePlusLighter;
                case CompositeDifference:
                    return kCGBlendModeDifference;
            }
            break;
        case BlendMode::Multiply:
            return kCGBlendModeMultiply;
        case BlendMode::Screen:
            return kCGBlendModeScreen;
        case BlendMode::Overlay:
            return kCGBlendModeOverlay;
        case BlendMode::Darken:
            return kCGBlendModeDarken;
        case BlendMode::Lighten:
            return kCGBlendModeLighten;
        case BlendMode::ColorDodge:
            return kCGBlendModeColorDodge;
        case BlendMode::ColorBurn:
            return kCGBlendModeColorBurn;
        case BlendMode::HardLight:
            return kCGBlendModeHardLight;
        case BlendMode::SoftLight:
            return kCGBlendModeSoftLight;
        case BlendMode::Difference:
            return kCGBlendModeDifference;
        case BlendMode::Exclusion:
            return kCGBlendModeExclusion;
        case BlendMode::Hue:
            return kCGBlendModeHue;
        case BlendMode::Saturation:
            return kCGBlendModeSaturation;
        case BlendMode::Color:
            return kCGBlendModeColor;
        case BlendMode::Luminosity:
            return kCGBlendModeLuminosity;
        case BlendMode::PlusDarker:
            return kCGBlendModePlusDarker;
        case BlendMode::PlusLighter:
            return kCGBlendModePlusLighter;
    }

    return kCGBlendModeNormal;
}

void GraphicsContextCG::setCompositeOperation(CompositeOperator compositeOperator, BlendMode blendMode) {
    CGContextSetBlendMode(m_context.get(), selectCGBlendMode(compositeOperator, blendMode));
}

void GraphicsContextCG::save() {
    if (paintingDisabled())
        return;

    m_stack.push_back(m_state);

    CGContextSaveGState(m_context.get());
}

void GraphicsContextCG::restore() {
    if (paintingDisabled())
        return;

    if (m_stack.empty()) {
        AtomLogError("ERROR void GraphicsContext::restore() stack is empty");
        return;
    }

    m_state = m_stack.back();
    m_stack.pop_back();

    // Make sure we deallocate the state stack buffer when it goes empty.
    // Canvas elements will immediately save() again, but that goes into inline capacity.
    if (m_stack.empty())
        m_stack.clear();

    CGContextRestoreGState(m_context.get());
}

void GraphicsContextCG::flush() {
    CGContextFlush(m_context.get());
}

void GraphicsContextCG::beginCompositeLayer(float opacity) {
    if (paintingDisabled())
        return;

    save();

    CGContextSetAlpha(m_context.get(), opacity);
    CGContextBeginTransparencyLayer(m_context.get(), 0);
}

void GraphicsContextCG::endCompositeLayer() {
    if (paintingDisabled())
        return;

    CGContextEndTransparencyLayer(m_context.get());

    restore();
}

bool GraphicsContextCG::paintingDisabled() {
    return !m_context;
}

FloatRect GraphicsContextCG::roundToDevicePixels(const FloatRect &rect, RoundingMode roundingMode) {
    if (paintingDisabled())
        return rect;


    CGAffineTransform deviceMatrix = CGContextGetUserSpaceToDeviceSpaceTransform(m_context.get());
    if (CGAffineTransformIsIdentity(deviceMatrix)) {
        return roundedIntRect(rect);
    }

    float deviceScaleX = sqrtf(deviceMatrix.a * deviceMatrix.a + deviceMatrix.b * deviceMatrix.b);
    float deviceScaleY = sqrtf(deviceMatrix.c * deviceMatrix.c + deviceMatrix.d * deviceMatrix.d);

    CGPoint deviceOrigin = CGPointMake(rect.x() * deviceScaleX, rect.y() * deviceScaleY);
    CGPoint deviceLowerRight = CGPointMake((rect.x() + rect.width()) * deviceScaleX,
                                           (rect.y() + rect.height()) * deviceScaleY);

    deviceOrigin.x = roundf(deviceOrigin.x);
    deviceOrigin.y = roundf(deviceOrigin.y);
    if (roundingMode == RoundAllSides) {
        deviceLowerRight.x = roundf(deviceLowerRight.x);
        deviceLowerRight.y = roundf(deviceLowerRight.y);
    } else {
        deviceLowerRight.x = deviceOrigin.x + roundf(rect.width() * deviceScaleX);
        deviceLowerRight.y = deviceOrigin.y + roundf(rect.height() * deviceScaleY);
    }

    // Don't let the height or width round to 0 unless either was originally 0
    if (deviceOrigin.y == deviceLowerRight.y && rect.height())
        deviceLowerRight.y += 1;
    if (deviceOrigin.x == deviceLowerRight.x && rect.width())
        deviceLowerRight.x += 1;

    FloatPoint roundedOrigin = FloatPoint(deviceOrigin.x / deviceScaleX, deviceOrigin.y / deviceScaleY);
    FloatPoint roundedLowerRight = FloatPoint(deviceLowerRight.x / deviceScaleX, deviceLowerRight.y / deviceScaleY);
    return FloatRect(roundedOrigin, (roundedLowerRight - roundedOrigin));
}

void GraphicsContextCG::setImageInterpolationQuality(InterpolationQuality quality) {
    m_state.imageInterpolationQuality = quality;

    CGInterpolationQuality q = kCGInterpolationDefault;
    switch (quality) {
        case InterpolationDefault:
            q = kCGInterpolationDefault;
            break;
        case InterpolationNone:
            q = kCGInterpolationNone;
            break;
        case InterpolationLow:
            q = kCGInterpolationLow;
            break;
        case InterpolationMedium:
            q = kCGInterpolationMedium;
            break;
        case InterpolationHigh:
            q = kCGInterpolationHigh;
            break;
    }

    CGContextSetInterpolationQuality(m_context.get(), q);
}

}