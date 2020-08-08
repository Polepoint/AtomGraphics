//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <cstring>
#include "CanvasTypes.h"

using namespace std;

namespace AtomGraphics {

static const char *const compositeOperatorNames[] = {
        "clear",
        "copy",
        "source-over",
        "source-in",
        "source-out",
        "source-atop",
        "destination-over",
        "destination-in",
        "destination-out",
        "destination-atop",
        "xor",
        "darker",
        "lighter",
        "difference"
};

static const char *const blendOperatorNames[] = {
        "normal",
        "multiply",
        "screen",
        "darken",
        "lighten",
        "overlay",
        "color-dodge",
        "color-burn",
        "hard-light",
        "soft-light",
        "difference",
        "exclusion",
        "hue",
        "saturation",
        "color",
        "luminosity",
        "plus-darker",
        "plus-lighter"
};

template<typename T, size_t Size>
char (&ArrayLengthHelperFunction(T (&)[Size]))[Size];

#define AT_ARRAY_LENGTH(array) sizeof(::AtomGraphics::ArrayLengthHelperFunction(array))

const int numCompositeOperatorNames = AT_ARRAY_LENGTH(compositeOperatorNames);
const int numBlendOperatorNames = AT_ARRAY_LENGTH(blendOperatorNames);

bool parseBlendMode(const string &s, BlendMode &blendMode) {
    for (int i = 0; i < numBlendOperatorNames; i++) {
        if (s == blendOperatorNames[i]) {
            blendMode = static_cast<BlendMode>(i + static_cast<unsigned>(BlendMode::Normal));
            return true;
        }
    }
    
    return false;
}

bool parseCompositeAndBlendOperator(const string &s, CompositeOperator &op, BlendMode &blendOp) {
    for (int i = 0; i < numCompositeOperatorNames; i++) {
        if (s == compositeOperatorNames[i]) {
            op = static_cast<CompositeOperator>(i);
            blendOp = BlendMode::Normal;
            return true;
        }
    }
    
    if (parseBlendMode(s, blendOp)) {
        // For now, blending will always assume source-over. This will be fixed in the future
        op = CompositeSourceOver;
        return true;
    }
    
    return false;
}

bool parseLineJoin(const string &s, LineJoin &lineJoin) {
    if (s == "round") {
        lineJoin = LineJoin::RoundJoin;
    } else if (s == "bevel") {
        lineJoin = LineJoin::BevelJoin;
    } else if (s == "miter") {
        lineJoin = LineJoin::MiterJoin;
    } else {
        return false;
    }
    
    return true;
}

bool parseLineCap(const string &s, LineCap &lineCap) {
    if (s == "round") {
        lineCap = LineCap::RoundCap;
    } else if (s == "butt") {
        lineCap = LineCap::ButtCap;
    } else if (s == "square") {
        lineCap = LineCap::SquareCap;
    } else {
        return false;
    }
    
    return true;
}

bool parseFillRule(const char *ruleStr, WindRule &rule) {
    if (std::strcmp(ruleStr, "nonzero") == 0) {
        rule = WindRule::NonZero;
        return true;
    } else if (std::strcmp(ruleStr, "evenodd") == 0) {
        rule = WindRule::EvenOdd;
        return true;
    }
    
    return false;
}

bool parseTextAlign(const std::string &s, TextAlign &textAlign) {
    if (s == "center") {
        textAlign = CenterTextAlign;
    } else if (s == "end") {
        textAlign = EndTextAlign;
    } else if (s == "left") {
        textAlign = LeftTextAlign;
    } else if (s == "right") {
        textAlign = RightTextAlign;
    } else if (s == "start") {
        textAlign = StartTextAlign;
    } else {
        return false;
    }
    
    return true;
}

bool parseTextBaseline(const std::string &s, TextBaseline &baseline) {
    
    if (s == "alphabetic") {
        baseline = AlphabeticTextBaseline;
    } else if (s == "top") {
        baseline = TopTextBaseline;
        
    } else if (s == "hanging") {
        baseline = HangingTextBaseline;
        
    } else if (s == "middle") {
        baseline = MiddleTextBaseline;
        
    } else if (s == "ideographic") {
        baseline = IdeographicTextBaseline;
        
    } else if (s == "bottom") {
        baseline = BottomTextBaseline;
        
    } else {
        return false;
    }
    
    return true;
}
}