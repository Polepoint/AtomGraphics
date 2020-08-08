//
// Created by neo on 2018/12/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSTYPES_H
#define ATOMGRAPHICS_GRAPHICSTYPES_H

namespace AtomGraphics {


enum LineCap {
    ButtCap, RoundCap, SquareCap
};

enum LineJoin {
    MiterJoin, RoundJoin, BevelJoin
};

enum CompositeOperator {
    CompositeClear,
    CompositeCopy,
    CompositeSourceOver,
    CompositeSourceIn,
    CompositeSourceOut,
    CompositeSourceAtop,
    CompositeDestinationOver,
    CompositeDestinationIn,
    CompositeDestinationOut,
    CompositeDestinationAtop,
    CompositeXOR,
    CompositePlusDarker,
    CompositePlusLighter,
    CompositeDifference
};

enum class BlendMode {
    Normal = 1,
    Multiply,
    Screen,
    Darken,
    Lighten,
    Overlay,
    ColorDodge,
    ColorBurn,
    HardLight,
    SoftLight,
    Difference,
    Exclusion,
    Hue,
    Saturation,
    Color,
    Luminosity,
    PlusDarker,
    PlusLighter
};

enum ImageSmoothingQuality {
    Low, Medium, High
};

enum InterpolationQuality {
    InterpolationDefault,
    InterpolationNone,
    InterpolationLow,
    InterpolationMedium,
    InterpolationHigh
};


enum TextAlign {
    StartTextAlign, EndTextAlign, LeftTextAlign, CenterTextAlign, RightTextAlign
};

enum TextBaseline {
    AlphabeticTextBaseline, TopTextBaseline, MiddleTextBaseline, BottomTextBaseline, IdeographicTextBaseline, HangingTextBaseline
};

enum class TextDirection : uint8_t {
    LTR, RTL
};

enum class AlphaPremultiplication {
    Premultiplied,
    Unpremultiplied
};

enum class WindRule : uint8_t {
    NonZero = 0,
    EvenOdd = 1
};

enum ColorFilter {
    kColorFilterNone,
    kColorFilterLuminanceToAlpha,
    kColorFilterSRGBToLinearRGB,
    kColorFilterLinearRGBToSRGB
};

enum ShadowMode {
    kDrawShadowAndForeground,
    kDrawShadowOnly,
    kDrawForegroundOnly
};

enum StrokeStyle {
    kNoStroke,
    kSolidStroke,
    kDottedStroke,
    kDashedStroke,
    kDoubleStroke,
    kWavyStroke,
};

enum TextDrawingMode {
    kTextModeFill = 1 << 0,
    kTextModeStroke = 1 << 1,
};


enum AccelerationHint {
    kPreferAcceleration,
    // The PreferAccelerationAfterVisibilityChange hint suggests we should switch
    // back to acceleration in the context of the canvas becoming visible again.
    kPreferAccelerationAfterVisibilityChange,
    kPreferNoAcceleration,
};

// Note: enum used directly for histogram, values must not change
enum DisableDeferralReason {
    kDisableDeferralReasonUnknown =
    0,  // Should not appear in production histograms
    kDisableDeferralReasonExpensiveOverdrawHeuristic = 1,
    kDisableDeferralReasonUsingTextureBackedPattern = 2,
    kDisableDeferralReasonDrawImageOfVideo = 3,
    kDisableDeferralReasonDrawImageOfAnimated2dCanvas = 4,
    kDisableDeferralReasonSubPixelTextAntiAliasingSupport = 5,
    kDisableDeferralDrawImageWithTextureBackedSourceImage = 6,
    kDisableDeferralReasonLowEndDevice = 7,
    kDisableDeferralReasonCount,
};

}


#endif //ATOMGRAPHICS_GRAPHICSTYPES_H
