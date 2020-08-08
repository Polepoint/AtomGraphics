#import "Pattern.h"
#import "graphics/Image.h"
#import "graphics/ios/CG/GraphicsContextCG.h"
#import "ios/CoreGraphicsAPIExtension.h"

namespace AtomGraphics {

static void patternCallback(void *info, CGContextRef context) {
    CGImageRef platformImage = static_cast<CGImageRef>(info);
    if (!platformImage)
        return;

    CGRect rect = GraphicsContextCG(context).roundToDevicePixels(
            FloatRect(0, 0, CGImageGetWidth(platformImage), CGImageGetHeight(platformImage)));
    CGContextDrawImage(context, rect, platformImage);
}

static void patternReleaseCallback(void *info) {
    //TODO: release implementation
//    callOnMainThread([image = static_cast<CGImageRef>(info)] {
//        CGImageRelease(image);
//    });
}

CGPatternRef Pattern::createPlatformPattern(const AffineTransform &userSpaceTransformation) const {
    FloatRect tileRect({0, 0}, m_tileImage->size());

    AffineTransform patternTransform = userSpaceTransformation * m_patternSpaceTransformation;
    patternTransform.scale(1, -1);
    patternTransform.translate(0, -tileRect.height());

    // If we're repeating in both directions, we can use image-backed patterns
    // instead of custom patterns, and avoid tiling-edge pixel cracks.
    if (m_repeatX && m_repeatY)
        return PatternCreateWithImage(m_tileImage->getCGImage(), patternTransform, kCGPatternTilingConstantSpacing);

    // If FLT_MAX should also be used for xStep or yStep, nothing is rendered. Using fractions of FLT_MAX also
    // result in nothing being rendered.
    // INT_MAX is almost correct, but there seems to be some number wrapping occurring making the fill
    // pattern is not filled correctly.
    // To make error of floating point less than 0.5, we use the half of the number of mantissa of float (1 << 22).
    CGFloat xStep = m_repeatX ? tileRect.width() : (1 << 22);
    CGFloat yStep = m_repeatY ? tileRect.height() : (1 << 22);

    // The pattern will release the CGImageRef when it's done rendering in patternReleaseCallback

    const CGPatternCallbacks patternCallbacks = {0, patternCallback, patternReleaseCallback};
    return CGPatternCreate(m_tileImage->getCGImage(), tileRect, patternTransform, xStep, yStep, kCGPatternTilingConstantSpacing, TRUE, &patternCallbacks);
}

}