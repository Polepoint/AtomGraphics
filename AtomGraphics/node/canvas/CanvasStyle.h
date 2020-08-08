//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CANVASSTYLE_H
#define ATOMGRAPHICS_CANVASSTYLE_H

#include "base/AtomTypes.h"
#include "graphics/Gradient.h"
#include "graphics/Pattern.h"
#include "platform/AtomPlatformConfig.h"

namespace AtomGraphics {

class CanvasStyle {

public:

    CanvasStyle();

    CanvasStyle(const Color4F &color);

    CanvasStyle(std::shared_ptr<Gradient> canvasGradient);

    CanvasStyle(std::shared_ptr<Pattern> canvasPattern);

    CanvasStyle(const CanvasStyle &style);

    bool isColorStyle() const;

    bool isGradientStyle() const;

    bool isPatternStyle() const;

    const Color4F &getColor() const;

    std::shared_ptr<Pattern> getCanvasPattern() const;

    std::shared_ptr<Gradient> getCanvasGradient() const;

#if PLATFORM(ANDROID)

    void applyToPaint(SkPaint &paint);

#endif

    CanvasStyle &operator=(const CanvasStyle &other);

    bool operator==(const CanvasStyle &rhs) const;

    bool operator!=(const CanvasStyle &rhs) const;

private:
    bool m_isColorStyle{false};
    bool m_isGradientStyle{false};
    bool m_isPatternStyle{false};

    Color4F m_color = Color4F::BLACK;
    std::shared_ptr<Pattern> m_pattern;
    std::shared_ptr<Gradient> m_gradient;

};

}


#endif //ATOMGRAPHICS_CANVASSTYLE_H
