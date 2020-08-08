package com.neo.atomgraphics.node.canvas;

public class CanvasType {

    public enum LineCap {
        ButtCap, RoundCap, SquareCap
    }

    public enum LineJoin {
        MiterJoin, RoundJoin, BevelJoin
    }

    public enum WindRule {
        NonZero,
        EvenOdd
    }

    public enum TextAlign {
        StartTextAlign,
        EndTextAlign,
        LeftTextAlign,
        CenterTextAlign,
        RightTextAlign
    }

    public enum TextBaseline {
        AlphabeticTextBaseline,
        TopTextBaseline,
        MiddleTextBaseline,
        BottomTextBaseline,
        IdeographicTextBaseline,
        HangingTextBaseline
    }

    public enum CompositeOperator {
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
    }

    public enum BlendMode {
        Normal,
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
    }
}
