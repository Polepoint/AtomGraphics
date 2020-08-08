//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "CanvasRenderingContext2DCG.h"
#import "CSSParser.h"
#import "GraphicsContextStateSaver.h"
#import "graphics/GraphicsLayer.h"

namespace AtomGraphics {

CanvasRenderingContext2DCG::CanvasRenderingContext2DCG(CanvasNode *node)
        : CanvasRenderingContext2D(node) {
    m_stateStack.push_back(CanvasRenderingContext2DState());
    ensurePlatformPath();
};

CanvasRenderingContext2DCG::~CanvasRenderingContext2DCG() {

}

void CanvasRenderingContext2DCG::setFillStyle(const CanvasStyle &canvasStyle) {
    if (currentState().fillStyle == canvasStyle) {
        return;
    }

    realizeSaves();
    currentState().fillStyle = canvasStyle;

    auto context = drawingContext();
    if (context) {
        context->setFillStyle(canvasStyle);
    }
}

void CanvasRenderingContext2DCG::setStrokeStyle(const CanvasStyle &canvasStyle) {
    if (currentState().strokeStyle == canvasStyle) {
        return;
    }

    realizeSaves();
    currentState().strokeStyle = canvasStyle;
    auto context = drawingContext();
    if (context) {
        context->setStrokeStyle(canvasStyle);
    }
}


void CanvasRenderingContext2DCG::setShadowColor(const Color4F &color) {
    auto context = drawingContext();
    if (!context)
        return;
    if (currentState().shadowColor == color) {
        return;
    }

    realizeSaves();
    currentState().shadowColor = color;
    applyShadow();
}

void CanvasRenderingContext2DCG::setShadowBlur(float blur) {
    if (!(std::isfinite(blur) && blur >= 0))
        return;
    if (currentState().shadowBlur == blur)
        return;
    realizeSaves();
    currentState().shadowBlur = blur;
    applyShadow();
}

void CanvasRenderingContext2DCG::setShadowOffsetX(float offsetX) {
    if (currentState().shadowOffset.width == offsetX) {
        return;
    }
    realizeSaves();
    currentState().shadowOffset.width = offsetX;
    applyShadow();
}

void CanvasRenderingContext2DCG::setShadowOffsetY(float offsetY) {
    if (currentState().shadowOffset.height == offsetY) {
        return;
    }
    realizeSaves();
    currentState().shadowOffset.height = offsetY;
    applyShadow();
}

void CanvasRenderingContext2DCG::applyShadow() {
    if (!drawingContext())
        return;

    CanvasRenderingContext2DState &curState = currentState();
    if (shouldDrawShadows()) {
        float width = curState.shadowOffset.width;
        float height = curState.shadowOffset.height;
        applyShadow(FloatSize(width, -height), curState.shadowBlur, curState.shadowColor);
    } else
        applyShadow(FloatSize::ZERO, 0, Color4F::TRANSPARENT);
}

void CanvasRenderingContext2DCG::applyShadow(const FloatSize &offset, const float blurRadius, const Color4F &shadowColor) {
    drawingContext()->setShadow(offset, blurRadius, shadowColor);
}

void CanvasRenderingContext2DCG::setLineCap(const LineCap lineCap) {
    if (currentState().lineCap == lineCap) {
        return;
    }

    realizeSaves();
    currentState().lineCap = lineCap;

    auto context = drawingContext();
    if (!context)
        return;
    context->setLineCap(lineCap);

}

void CanvasRenderingContext2DCG::setLineJoin(const LineJoin lineJoin) {
    if (currentState().lineJoin == lineJoin) {
        return;
    }

    realizeSaves();
    currentState().lineJoin = lineJoin;

    auto context = drawingContext();
    if (!context)
        return;
    context->setLineJoin(lineJoin);
}

void CanvasRenderingContext2DCG::setLineWidth(float width) {
    if (!(std::isfinite(width) && width > 0))
        return;

    if (currentState().lineWidth == width) {
        return;
    }

    realizeSaves();
    currentState().lineWidth = width;

    auto context = drawingContext();
    if (!context) {
        return;
    }
    context->setStrokeThickness(width);
}

static bool lineDashSequenceIsValid(const std::vector<float> &dash) {
    for (size_t i = 0; i < dash.size(); i++) {
        if (!std::isfinite(dash[i]) || dash[i] < 0)
            return false;
    }
    return true;
}

void CanvasRenderingContext2DCG::setLineDash(const std::vector<float> &dashArray) {
    if (!lineDashSequenceIsValid(dashArray))
        return;

    realizeSaves();
    auto &curState = currentState();
    curState.lineDash = dashArray;
    // Spec requires the concatenation of two copies the dash list when the
    // number of elements is odd
    if (dashArray.size() % 2) {
        curState.lineDash.insert(curState.lineDash.end(), dashArray.begin(), dashArray.end());
    }

    applyLineDash();
}

void CanvasRenderingContext2DCG::setLineDashOffset(float offset) {
    if (!std::isfinite(offset) || currentState().lineDashOffset == offset)
        return;

    realizeSaves();
    currentState().lineDashOffset = offset;
    applyLineDash();
}

void CanvasRenderingContext2DCG::applyLineDash() {
    GraphicsContext *c = drawingContext();
    if (!c)
        return;
    auto &curState = currentState();
    DashArray convertedLineDash(curState.lineDash.size());
    for (size_t i = 0; i < curState.lineDash.size(); ++i)
        convertedLineDash[i] = static_cast<DashArrayElement>(curState.lineDash[i]);
    c->setLineDash(convertedLineDash, currentState().lineDashOffset);
}

void CanvasRenderingContext2DCG::setMiterLimit(float limit) {
    if (!(std::isfinite(limit) && limit > 0))
        return;

    if (currentState().miterLimit == limit) {
        return;
    }
    realizeSaves();
    currentState().miterLimit = limit;

    auto context = drawingContext();
    if (!context) {
        return;
    }

    context->setMiterLimit(limit);
}

void CanvasRenderingContext2DCG::rect(float x, float y, float width, float height) {
    if (!currentState().hasInvertibleTransform)
        return;

    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(width) || !std::isfinite(height))
        return;

    if (!width && !height) {
        m_path->moveTo(x, y);
        return;
    }

    m_path->addRect(FloatRect(x, y, width, height));
}

static bool validateRectForCanvas(float &x, float &y, float &width, float &height) {
    if (!std::isfinite(x) | !std::isfinite(y) | !std::isfinite(width) | !std::isfinite(height))
        return false;

    if (!width && !height)
        return false;

    if (width < 0) {
        width = -width;
        x -= width;
    }

    if (height < 0) {
        height = -height;
        y -= height;
    }

    return true;
}

bool CanvasRenderingContext2DCG::isFullCanvasCompositeMode(CompositeOperator op) {
    return op == CompositeSourceIn || op == CompositeSourceOut || op == CompositeDestinationIn || op == CompositeDestinationAtop;
}


void CanvasRenderingContext2DCG::fillRect(float x, float y, float width, float height) {
    if (!validateRectForCanvas(x, y, width, height))
        return;

    auto context = drawingContext();
    if (!context)
        return;
    CanvasRenderingContext2DState &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    // from the HTML5 Canvas spec:
    // If x0 = x1 and y0 = y1, then the linear gradient must paint nothing
    // If x0 = x1 and y0 = y1 and r0 = r1, then the radial gradient must paint nothing
    auto &fillStyle = curState.fillStyle;
    if (fillStyle.isGradientStyle() && fillStyle.getCanvasGradient()->isZeroSize())
        return;

    FloatRect rect(x, y, width, height);

    if (rectContainsCanvas(rect)) {
        context->fillRect(rect);
        didDrawEntireCanvas();
    } else if (isFullCanvasCompositeMode(curState.globalComposite)) {
        context->beginCompositeLayer();
        context->fillRect(rect);
        context->endCompositeLayer();
        didDrawEntireCanvas();
    } else if (curState.globalComposite == CompositeCopy) {
        clearCanvas();
        context->fillRect(rect);
        didDrawEntireCanvas();
    } else {
        context->fillRect(rect);
        didDraw(rect);
    }
}


void CanvasRenderingContext2DCG::strokeRect(float x, float y, float width, float height) {
    if (!validateRectForCanvas(x, y, width, height))
        return;

    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;
    if (currentState().lineWidth < 0)
        return;

    // If gradient size is zero, then paint nothing.
    auto &strokeStyle = currentState().strokeStyle;
    if (strokeStyle.isGradientStyle() && strokeStyle.getCanvasGradient()->isZeroSize())
        return;

    FloatRect rect(x, y, width, height);
    if (isFullCanvasCompositeMode(currentState().globalComposite)) {
        context->beginCompositeLayer();
        context->strokeRect(rect, 0);
        context->endCompositeLayer();
        didDrawEntireCanvas();
    } else if (currentState().globalComposite == CompositeCopy) {
        clearCanvas();
        context->strokeRect(rect, 0);
        didDrawEntireCanvas();
    } else {
        FloatRect boundingRect = rect;
        boundingRect.inflate(currentState().lineWidth / 2);
        context->strokeRect(rect, 0);
        didDraw(boundingRect);
    }

}

bool CanvasRenderingContext2DCG::shouldDrawShadows() {
    CanvasRenderingContext2DState &curState = currentState();
    return curState.shadowColor != Color4F::TRANSPARENT && (curState.shadowBlur || !curState.shadowOffset.equals(FloatSize::ZERO));
}

bool CanvasRenderingContext2DCG::rectContainsCanvas(const FloatRect &rect) {
    FloatQuad quad(rect);
    FloatSize size = contentSize();
    FloatQuad canvasQuad(FloatRect(0, 0, size.width, size.height));
    return currentState().transform.mapQuad(quad).containsQuad(canvasQuad);
}


void CanvasRenderingContext2DCG::clearRect(float x, float y, float width, float height) {
    if (!validateRectForCanvas(x, y, width, height))
        return;

    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;
    FloatRect rect(x, y, width, height);

    bool saved = false;
    if (shouldDrawShadows()) {
        context->save();
        saved = true;
        context->setShadow(FloatSize(), 0, Color4F::TRANSPARENT);
    }

    if (currentState().globalAlpha != 1) {
        if (!saved) {
            context->save();
            saved = true;
        }
        context->setGlobalAlpha(1);
    }
    if (currentState().globalComposite != CompositeSourceOver) {
        if (!saved) {
            context->save();
            saved = true;
        }
        context->setCompositeOperation(CompositeSourceOver);
    }
    context->clearRect(rect);
    if (saved)
        context->restore();
    didDraw(rect);
}

void CanvasRenderingContext2DCG::fill(WindRule fillRule) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    // If gradient size is zero, then paint nothing.
    auto &fillStyle = currentState().fillStyle;
    if (fillStyle.isGradientStyle() && fillStyle.getCanvasGradient()->isZeroSize())
        return;

    if (!m_path->isEmpty()) {
        auto savedFillRule = context->fillRule();
        context->setFillRule(fillRule);

        if (isFullCanvasCompositeMode(currentState().globalComposite)) {
            context->beginCompositeLayer();
            context->fillPath(*m_path.get());
            context->endCompositeLayer();
            didDrawEntireCanvas();
        } else if (currentState().globalComposite == CompositeCopy) {
            clearCanvas();
            context->fillPath(*m_path.get());
            didDrawEntireCanvas();
        } else {
            context->fillPath(*m_path.get());
            didDraw(m_path->fastBoundingRect());
        }

        context->setFillRule(savedFillRule);
    }
}

void CanvasRenderingContext2DCG::stroke() {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    // If gradient size is zero, then paint nothing.
    auto &strokeStyle = currentState().strokeStyle;
    if (strokeStyle.isGradientStyle() && strokeStyle.getCanvasGradient()->isZeroSize())
        return;

    if (!m_path->isEmpty()) {
        if (isFullCanvasCompositeMode(currentState().globalComposite)) {
            context->beginCompositeLayer();
            context->strokePath(m_path.get());
            context->endCompositeLayer();
            didDrawEntireCanvas();
        } else if (currentState().globalComposite == CompositeCopy) {
            clearCanvas();
            context->strokePath(m_path.get());
            didDrawEntireCanvas();
        } else {
            FloatRect dirtyRect = m_path->fastBoundingRect();
            inflateStrokeRect(dirtyRect);
            context->strokePath(m_path.get());
            didDraw(dirtyRect);
        }
    }
}

void CanvasRenderingContext2DCG::inflateStrokeRect(FloatRect &rect) {
    // Fast approximation of the stroke's bounding rect.
    // This yields a slightly oversized rect but is very fast
    // compared to Path::strokeBoundingRect().
    static const float root2 = sqrtf(2);
    float delta = currentState().lineWidth / 2;
    if (currentState().lineJoin == MiterJoin)
        delta *= currentState().miterLimit;
    else if (currentState().lineCap == SquareCap)
        delta *= root2;
    rect.inflate(delta);
}

void CanvasRenderingContext2DCG::beginPath() {
    m_path->clear();
}

void CanvasRenderingContext2DCG::moveTo(float x, float y) {
    if (!std::isfinite(x) || !std::isfinite(y))
        return;
    if (!currentState().hasInvertibleTransform)
        return;
    m_path->moveTo(x, y);
}

void CanvasRenderingContext2DCG::closePath() {
    if (m_path->isEmpty())
        return;

    FloatRect boundRect = m_path->fastBoundingRect();
    if (boundRect.width() || boundRect.height())
        m_path->closePath();
}

void CanvasRenderingContext2DCG::lineTo(float x, float y) {
    if (!std::isfinite(x) || !std::isfinite(y))
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    FloatPoint p1 = FloatPoint(x, y);
    if (!m_path->hasCurrentPoint())
        m_path->moveTo(x, y);
    else if (p1 != m_path->currentPoint())
        m_path->addLineTo(x, y);
}

void CanvasRenderingContext2DCG::clip(WindRule windingRule) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    realizeSaves();
    context->clipPath(m_path.get(), windingRule);
}

void CanvasRenderingContext2DCG::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    if (!std::isfinite(cpx) || !std::isfinite(cpy) || !std::isfinite(x) || !std::isfinite(y))
        return;
    if (!currentState().hasInvertibleTransform)
        return;
    if (!m_path->hasCurrentPoint())
        m_path->moveTo(cpx, cpy);

    FloatPoint p1 = FloatPoint(x, y);
    FloatPoint cp = FloatPoint(cpx, cpy);
    if (p1 != m_path->currentPoint() || p1 != cp)
        m_path->addQuadCurveTo(cpx, cpy, x, y);
}

void CanvasRenderingContext2DCG::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    if (!std::isfinite(cp1x) || !std::isfinite(cp1y) || !std::isfinite(cp2x) || !std::isfinite(cp2y) || !std::isfinite(x) || !std::isfinite(y))
        return;
    if (!currentState().hasInvertibleTransform)
        return;
    if (!m_path->hasCurrentPoint())
        m_path->moveTo(cp1x, cp1y);

    FloatPoint p1 = FloatPoint(x, y);
    FloatPoint cp1 = FloatPoint(cp1x, cp1y);
    FloatPoint cp2 = FloatPoint(cp2x, cp2y);
    if (p1 != m_path->currentPoint() || p1 != cp1 || p1 != cp2)
        m_path->addBezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

static void normalizeAngles(float &startAngle, float &endAngle, bool anticlockwise) {
    float newStartAngle = startAngle;
    if (newStartAngle < 0)
        newStartAngle = (2 * piFloat) + fmodf(newStartAngle, -(2 * piFloat));
    else
        newStartAngle = fmodf(newStartAngle, 2 * piFloat);

    float delta = newStartAngle - startAngle;
    startAngle = newStartAngle;
    endAngle = endAngle + delta;

    if (anticlockwise && startAngle - endAngle >= 2 * piFloat)
        endAngle = startAngle - 2 * piFloat;
    else if (!anticlockwise && endAngle - startAngle >= 2 * piFloat)
        endAngle = startAngle + 2 * piFloat;
}

void CanvasRenderingContext2DCG::arc(float x, float y, float radius, float startAngle, float endAngle, bool counterclockwise) {
    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(radius) || !std::isfinite(startAngle) || !std::isfinite(endAngle))
        return;

    if (radius < 0)
        return;

    if (!currentState().hasInvertibleTransform)
        return;

    normalizeAngles(startAngle, endAngle, counterclockwise);

    if (!radius || startAngle == endAngle) {
        lineTo(x + radius * cosf(startAngle), y + radius * sinf(startAngle));
        return;
    }

    m_path->addArc(x, y, radius, startAngle, endAngle, counterclockwise);
}

void CanvasRenderingContext2DCG::arcTo(float x1, float y1, float x2, float y2, float radius) {
    if (!std::isfinite(x1) || !std::isfinite(y1) || !std::isfinite(x2) || !std::isfinite(y2) || !std::isfinite(radius))
        return;

    if (radius < 0)
        return;

    if (!currentState().hasInvertibleTransform)
        return;

    FloatPoint p1 = FloatPoint(x1, y1);
    FloatPoint p2 = FloatPoint(x2, y2);

    if (!m_path->hasCurrentPoint())
        m_path->moveTo(x1, y1);
    else if (p1 == m_path->currentPoint() || p1 == p2 || !radius)
        lineTo(x1, y1);
    else
        m_path->addArcToPoint(x1, y1, x2, y2, radius);
}

bool CanvasRenderingContext2DCG::isPointInPath(float x, float y) {
    auto context = drawingContext();
    if (!context)
        return false;
    if (!currentState().hasInvertibleTransform)
        return false;

    FloatPoint transformedPoint;

    AffineTransform ctm;
    if (currentState().transform.inverse(ctm)) {
        transformedPoint = ctm.mapPoint(x, y);
    } else {
        transformedPoint = FloatPoint(x, y);
    }

    if (!std::isfinite(transformedPoint.x) || !std::isfinite(transformedPoint.y))
        return false;

    return m_path->containsPoint(transformedPoint.x, transformedPoint.y);
}

void CanvasRenderingContext2DCG::scale(float sx, float sy) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    if (!std::isfinite(sx) || !std::isfinite(sy))
        return;

    AffineTransform newTransform = currentState().transform;
    newTransform.scale(sx, sy);
    if (currentState().transform == newTransform)
        return;

    realizeSaves();

    if (!sx || !sy) {
        currentState().hasInvertibleTransform = false;
        return;
    }

    currentState().transform = newTransform;
    context->scale(sx, sy);
    m_path->transform(AffineTransform().scale(1.0 / sx, 1.0 / sy));
}

void CanvasRenderingContext2DCG::rotate(float angleInRadians) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    if (!std::isfinite(angleInRadians))
        return;

    AffineTransform newTransform = currentState().transform;
    newTransform.rotate(angleInRadians / M_PI * 180.0);
    if (currentState().transform == newTransform)
        return;

    realizeSaves();

    currentState().transform = newTransform;
    context->rotate(angleInRadians);
    m_path->transform(AffineTransform().rotate(-angleInRadians / M_PI * 180.0));
}

void CanvasRenderingContext2DCG::translate(float tx, float ty) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    if (!std::isfinite(tx) | !std::isfinite(ty))
        return;

    AffineTransform newTransform = currentState().transform;
    newTransform.translate(tx, ty);
    if (currentState().transform == newTransform)
        return;

    realizeSaves();

    currentState().transform = newTransform;
    context->translate(tx, ty);
    m_path->transform(AffineTransform().translate(-tx, -ty));
}

void CanvasRenderingContext2DCG::transform(float a, float b, float c, float d, float tx, float ty) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    if (!std::isfinite(a) | !std::isfinite(b) | !std::isfinite(c) | !std::isfinite(d) | !std::isfinite(tx) | !std::isfinite(ty))
        return;

    AffineTransform transform(a, b, c, d, tx, ty);
    AffineTransform newTransform = currentState().transform * transform;
    if (currentState().transform == newTransform)
        return;

    realizeSaves();

    AffineTransform inverseValue;
    if (transform.inverse(inverseValue)) {
        currentState().transform = newTransform;
        context->concat(transform);
        m_path->transform(inverseValue);
        return;
    }

    currentState().hasInvertibleTransform = false;
}

void CanvasRenderingContext2DCG::setTransform(float a, float b, float c, float d, float tx, float ty) {
    auto context = drawingContext();
    if (!context)
        return;

    if (!std::isfinite(a) | !std::isfinite(b) | !std::isfinite(c) | !std::isfinite(d) | !std::isfinite(tx) | !std::isfinite(ty))
        return;

    resetTransform();
    transform(a, b, c, d, tx, ty);
}

void CanvasRenderingContext2DCG::resetTransform() {
    auto context = drawingContext();
    if (!context)
        return;

    AffineTransform ctm = currentState().transform;
    bool hasInvertibleTransform = currentState().hasInvertibleTransform;

    realizeSaves();

    context->setTransform(m_imageBuffer->baseTransform());
    currentState().transform = AffineTransform();

    if (hasInvertibleTransform)
        m_path->transform(ctm);

    currentState().hasInvertibleTransform = true;
}

void CanvasRenderingContext2DCG::setFont(const std::string &fontString) {
    CanvasRenderingContext2DState &curState = currentState();
    if (curState.unparsedFont == fontString) {
        return;
    }

    Font font;
    FontDescription description;
    if (!CSSParser::parseCSSString(fontString, description)) {
        return;
    }

    font.setFontDescription(description);
    curState.unparsedFont = fontString;
    curState.font = font;
}

std::string CanvasRenderingContext2DCG::getFont() {
    CanvasRenderingContext2DState &curState = currentState();
    if (!curState.unparsedFont.empty()) {
        return curState.unparsedFont;
    }

    return CSSParser::fontToCSSConfig(currentState().font);
}

void CanvasRenderingContext2DCG::setTextAlign(TextAlign textAlign) {
    CanvasRenderingContext2DState &curState = currentState();
    if (curState.textAlign == textAlign) {
        return;
    }
    realizeSaves();

    curState.textAlign = textAlign;
}

void CanvasRenderingContext2DCG::setTextBaseline(TextBaseline textBaseline) {
    CanvasRenderingContext2DState &curState = currentState();
    if (curState.textBaseline == textBaseline) {
        return;
    }

    realizeSaves();

    curState.textBaseline = textBaseline;

}

FloatPoint CanvasRenderingContext2DCG::textOffset(float width) {
    auto &fontMetrics = currentState().font.PrimaryFontData()->fontMetrics();
    FloatPoint offset = {0, 0};

    switch (currentState().textBaseline) {
        case TopTextBaseline:
        case HangingTextBaseline:
            offset.y = fontMetrics.ascent();
            break;
        case BottomTextBaseline:
        case IdeographicTextBaseline:
            offset.y = -fontMetrics.descent();
            break;
        case MiddleTextBaseline:
            offset.y = fontMetrics.height() / 2 - fontMetrics.descent();
            break;
        case AlphabeticTextBaseline:
        default:
            break;
    }

    auto align = currentState().textAlign;
    if (align == StartTextAlign)
        align = LeftTextAlign;
    else if (align == EndTextAlign)
        align = RightTextAlign;

    switch (align) {
        case CenterTextAlign:
            offset.x = -width / 2;
            break;
        case RightTextAlign:
            offset.x = -width;
            break;
        default:
            break;
    }

    return offset;
}

void CanvasRenderingContext2DCG::fillText(const base::String &text, float x, float y, float *maxWidth) {
    drawText(text, x, y, true, maxWidth);
}

void CanvasRenderingContext2DCG::strokeText(const base::String &text, float x, float y, float *maxWidth) {
    drawText(text, x, y, false, maxWidth);
}

static inline bool isSpaceThatNeedsReplacing(UChar c) {
    return c == 0x0009 || c == 0x000A || c == 0x000B || c == 0x000C || c == 0x000D;
}

static void normalizeSpaces(base::String &text) {
    size_t i = text.find(isSpaceThatNeedsReplacing);
    if (i == NotFound)
        return;

//    unsigned textLength = text.length();
//    std::vector<UChar> charVector(textLength);
//    StringView(text).getCharactersWithUpconvert(charVector.data());
//
//    charVector[i++] = ' ';
//
//    for (; i < textLength; ++i) {
//        if (isSpaceThatNeedsReplacing(charVector[i]))
//            charVector[i] = ' ';
//    }
//    text = String::adopt(WTFMove(charVector));
}

FloatPoint CanvasRenderingContext2DCG::textOffset(float width, TextDirection direction) {
    auto &fontMetrics = currentState().font.PrimaryFontData()->fontMetrics();
    FloatPoint offset;

    switch (currentState().textBaseline) {
        case TopTextBaseline:
        case HangingTextBaseline:
            offset.y = fontMetrics.ascent();
            break;
        case BottomTextBaseline:
        case IdeographicTextBaseline:
            offset.y = -fontMetrics.descent();
            break;
        case MiddleTextBaseline:
            offset.y = fontMetrics.height() / 2 - fontMetrics.descent();
            break;
        case AlphabeticTextBaseline:
        default:
            break;
    }

    bool isRTL = direction == TextDirection::RTL;
    auto align = currentState().textAlign;
    if (align == StartTextAlign)
        align = isRTL ? RightTextAlign : LeftTextAlign;
    else if (align == EndTextAlign)
        align = isRTL ? LeftTextAlign : RightTextAlign;

    switch (align) {
        case CenterTextAlign:
            offset.x = -width / 2;
            break;
        case RightTextAlign:
            offset.x = -width;
            break;
        default:
            break;
    }
    return offset;
}

void CanvasRenderingContext2DCG::drawText(const base::String &text, float x, float y, bool fill, float *maxWidth) {
    auto context = drawingContext();
    if (!context)
        return;

    CanvasRenderingContext2DState &curState = currentState();

    if (!curState.hasInvertibleTransform)
        return;
    if (!std::isfinite(x) | !std::isfinite(y))
        return;

    if (maxWidth && (!std::isfinite(*maxWidth) || *maxWidth < 0))
        return;

    auto &strokeStyle = curState.strokeStyle;
    if (!fill && strokeStyle.isGradientStyle() && strokeStyle.getCanvasGradient()->isZeroSize())
        return;

    auto &fillStyle = curState.fillStyle;
    if (fill && fillStyle.isGradientStyle() && fillStyle.getCanvasGradient()->isZeroSize())
        return;

    const auto &fontMetrics = curState.font.PrimaryFontData()->fontMetrics();

//    base::String normalizedText = text;
//    normalizeSpaces(normalizedText);

    float fontWidth = curState.font.widthForString(text);
    bool useMaxWidth = maxWidth != nullptr && *maxWidth < fontWidth;
    float width = useMaxWidth ? *maxWidth : fontWidth;

    FloatPoint location(x, y);
    location += textOffset(width, currentState().textDirection);

    FloatRect textRect = FloatRect(location.x - fontMetrics.height() / 2, location.y - fontMetrics.ascent() - fontMetrics.lineGap(),
                                   fontWidth + fontMetrics.height(), fontMetrics.lineSpacing());
    if (!fill)
        inflateStrokeRect(textRect);

    const CanvasStyle &drawStyle = fill ? curState.fillStyle : curState.strokeStyle;

    if (drawStyle.isGradientStyle() || drawStyle.isPatternStyle()) {
        IntRect maskRect = enclosingIntRect(textRect);

        // If we have a shadow, we need to draw it before the mask operation.
        // Follow a procedure similar to paintTextWithShadows in TextPainter.

        if (shouldDrawShadows()) {
            GraphicsContextStateSaver stateSaver(context);

            FloatSize offset(0, 2 * maskRect.height());

            FloatSize shadowOffset = curState.shadowOffset;
            float shadowRadius = curState.shadowBlur;
            Color4F shadowColor = curState.shadowColor;

            FloatRect shadowRect(maskRect);
            shadowRect.inflate(shadowRadius * 1.4f);
            shadowRect.move(shadowOffset * -1);
            context->clipRect(shadowRect);

            shadowOffset += offset;

            context->setShadow(shadowOffset, shadowRadius, shadowColor);

            if (fill)
                context->setFillStyle(CanvasStyle(Color4F::BLACK));
            else
                context->setStrokeStyle(CanvasStyle(Color4F::BLACK));

            context->drawText(text, location.x + offset.width, location.y + offset.height, curState.font, fill);
        }

        auto maskImage = ImageBuffer::Create(maskRect.size, 4);
        if (!maskImage)
            return;

        auto maskImageContext = maskImage->createGraphicsContext();

        if (fill)
            maskImageContext->setFillStyle(Color4F::BLACK);
        else {
            maskImageContext->setStrokeStyle(Color4F::BLACK);
            maskImageContext->setStrokeThickness(curState.lineWidth);
        }

        if (useMaxWidth) {
            maskImageContext->translate(location.x - maskRect.origin.x, location.y - maskRect.origin.y);
            // We draw when fontWidth is 0 so compositing operations (eg, a "copy" op) still work.
            FloatSize scaleSize = FloatSize((fontWidth > 0 ? (width / fontWidth) : 0), 1);
            maskImageContext->scale(scaleSize.width, scaleSize.height);
            maskImageContext->drawText(text, 0, 0, curState.font, fill);
        } else {
            maskImageContext->translate(-maskRect.origin.x, -maskRect.origin.y);
            maskImageContext->drawText(text, location.x, location.y, curState.font, fill);
        }

        GraphicsContextStateSaver stateSaver(context);
        context->clipToImageBuffer(*maskImage, maskRect);
        context->setFillStyle(drawStyle);
        context->fillRect(maskRect);
        maskImage->release();
        return;
    }

    GraphicsContextStateSaver stateSaver(context);

    if (useMaxWidth) {
        context->translate(location.x, location.y);
        // We draw when fontWidth is 0 so compositing operations (eg, a "copy" op) still work.
        context->scale((fontWidth > 0 ? (fontWidth / fontWidth) : 0), 1);
        location = FloatPoint();
    }

    if (isFullCanvasCompositeMode(curState.globalComposite)) {
        context->beginCompositeLayer();
        context->drawText(text, location.x, location.y, currentState().font, fill);
        context->endCompositeLayer();
        didDrawEntireCanvas();
    } else if (curState.globalComposite == CompositeCopy) {
        clearCanvas();
        context->drawText(text, location.x, location.y, currentState().font, fill);
        didDrawEntireCanvas();
    } else {
        context->drawText(text, location.x, location.y, currentState().font, fill);
        didDraw(textRect);
    }
}
//
//void CanvasContext2DCG::drawText(const std::string &text, float x, float y, float maxWidth, const TextDrawingMode drawingMode) {
//    if (!m_drawingContext)
//        return;
//    State &curState = currentState();
//    if (!curState.hasInvertibleTransform)
//        return;
//    if (!std::isfinite(x) | !std::isfinite(y))
//        return;
//    Font &font = curState.font;
//    CTFontRef ctFont = font.ctFont();
//    CGContextSaveGState(m_drawingContext);
//    CGRect rect = CTFontGetBoundingBox(ctFont);
//    NSString *textValue = [NSString stringWithCString:text.data() encoding:NSUTF8StringEncoding];
//    NSMutableAttributedString *attributedString = [[NSMutableAttributedString alloc] initWithString:textValue];
//
//    long number = MAX(curState.lineWidth, 1.0) * (drawingMode == TextFill ? -1.0 : 1.0);
//    CFNumberRef num = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt8Type, &number);
//    [attributedString addAttribute:(id) kCTStrokeWidthAttributeName value:(__bridge id) num range:NSMakeRange(0, [attributedString length])];
//
//    [attributedString addAttribute:(id) kCTFontAttributeName value:(__bridge id) ctFont range:NSMakeRange(0, attributedString.length)];
//    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
//    paragraphStyle.lineBreakMode = NSLineBreakByClipping;
//    CTFramesetterRef frameSetter = CTFramesetterCreateWithAttributedString((CFAttributedStringRef) attributedString);
//    CGSize contentSize = CTFramesetterSuggestFrameSizeWithConstraints(frameSetter, CFRangeMake(0, 0), nil,
//            CGSizeMake((CGFloat)(m_contentFrame.size.width * 2.f), ceil(rect.size.height) * 1.5), NULL);
//    float measureWidth = contentSize.width;
//    bool useMaxWidth = maxWidth > 0 && maxWidth < measureWidth;
//    float fontWidth = useMaxWidth ? maxWidth : measureWidth;
//
//    FloatSize targetSize;
//    targetSize.setSize(ceil(contentSize.width), ceil(contentSize.height));
//    targetSize.scale(m_deviceScaleFactor);
//    int clippedImageTotalLength = targetSize.width * targetSize.height * 4;
//    uint8_t *imageData = (uint8_t *) malloc(clippedImageTotalLength * sizeof(uint8_t));
//    memset(imageData, 0, clippedImageTotalLength);
//    CGContextRef context = CGBitmapContextCreate(imageData,
//            static_cast<size_t>(targetSize.width),
//            static_cast<size_t>(targetSize.height),
//            8,
//            static_cast<size_t>(targetSize.width * 4),
//            sRGBColorSpaceRef(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
//    CGContextScaleCTM(context, m_deviceScaleFactor, m_deviceScaleFactor);
//    CGContextConcatCTM(context, CGAffineTransformMake(1, 0, 0, -1, 0, targetSize.height));
//    CGMutablePathRef path = CGPathCreateMutable();
//    CGPathAddRect(path, NULL, CGRectMake(0, 0, ceil(targetSize.width), ceil(targetSize.height)));
//    NSInteger length = attributedString.length;
//    CTFrameRef frame = CTFramesetterCreateFrame(frameSetter, CFRangeMake(0, length), path, NULL);
//    CTFrameDraw(frame, context);
//
//    CFRelease(frame);
//    CFRelease(path);
//    CFRelease(frameSetter);
//
//    FloatPoint offset = textOffset(fontWidth);
//    CGImageRef mask = CGBitmapContextCreateImage(context);
//    free(imageData);
//    CGContextClipToMask(m_drawingContext, CGRectMake(x, y - contentSize.height / 2.0, ceil(contentSize.width), ceil(contentSize.height)), mask);
//    if (drawingMode == TextFill) {
//        fillRect(m_contentFrame.origin.x, m_contentFrame.origin.y, m_contentFrame.size.width, m_contentFrame.size.height);
//    } else {
//        CanvasStyle &strokeStyle = curState.strokeStyle;
//        if (strokeStyle.isGradientStyle()) {
//            CGContextSaveGState(m_drawingContext);
//            CanvasGradient canvasGradient = strokeStyle.getCanvasGradient();
//            drawGradient(canvasGradient);
//            CGContextRestoreGState(m_drawingContext);
//        } else if (strokeStyle.isColorStyle()) {
//            Color4F color = strokeStyle.getColor();
//            CGContextSetFillColorWithColor(m_drawingContext, cachedCGColor(color));
//            CGContextFillRect(m_drawingContext, CGRectMake(m_contentFrame.origin.x, m_contentFrame.origin.y, m_contentFrame.size.width, m_contentFrame.size.height));
//        }
//    }
//    CGContextRestoreGState(m_drawingContext);
//    CGContextRelease(context);
//    CGImageRelease(mask);
//    if (!m_dirtyRect.containsRect(m_contentFrame)) {
//        expandDirtyRectAndUpdateIfNeed(FloatRect(x, y - contentSize.height / 2.0, contentSize.width, contentSize.height));
//    }
//}

TextMetrics CanvasRenderingContext2DCG::measureText(const base::String &text) {
    TextMetrics metrics;
    metrics.setWidth(currentState().font.widthForString(text));
    return metrics;
}

static inline FloatRect normalizeRect(const FloatRect &rect) {
    return FloatRect(std::min(rect.x(), rect.getMaxX()),
                     std::min(rect.y(), rect.getMaxY()),
                     std::max(rect.width(), -rect.width()),
                     std::max(rect.height(), -rect.height()));
}

ImageBuffer *CanvasRenderingContext2DCG::createCompositingBuffer(const IntRect &bufferRect) {
    return ImageBuffer::Create(bufferRect.size);
}

Path CanvasRenderingContext2DCG::transformAreaToDevice(const Path &path) {
    Path transformed(path);
    transformed.transform(currentState().transform);
    transformed.transform(m_imageBuffer->baseTransform());
    return transformed;
}

Path CanvasRenderingContext2DCG::transformAreaToDevice(const FloatRect &rect) {
    Path path;
    path.addRect(rect);
    return transformAreaToDevice(path);
}

IntRect CanvasRenderingContext2DCG::calculateCompositingBufferRect(const FloatRect &area, IntSize *croppedOffset) {
    IntRect canvasRect(0, 0, m_node->contentSize().width, m_node->contentSize().height);
    canvasRect = m_imageBuffer->baseTransform().mapRect(canvasRect);
    Path path = transformAreaToDevice(area);
    IntRect bufferRect = enclosingIntRect(path.fastBoundingRect());
    IntPoint originalLocation = bufferRect.origin;
    bufferRect.intersect(canvasRect);
    if (croppedOffset)
        *croppedOffset = originalLocation - bufferRect.origin;
    return bufferRect;
}

void CanvasRenderingContext2DCG::compositeBuffer(ImageBuffer &buffer, const IntRect &bufferRect, CompositeOperator op) {
    IntRect canvasRect(0, 0, m_node->contentSize().width, m_node->contentSize().height);
    canvasRect = m_imageBuffer->baseTransform().mapRect(canvasRect);

    auto context = drawingContext();
    if (!context)
        return;

    context->save();
    context->setTransform(AffineTransform());
    context->setCompositeOperation(op);

    context->save();
    context->clipOut(bufferRect);
    context->clearRect(canvasRect);
    context->restore();
    scoped_refptr<Image> image = buffer.copyNativeImage();
    context->drawImage(image.get(),
                       {bufferRect.origin, buffer.resolutionSize()},
                       {FloatPoint(), buffer.resolutionSize()},
                       currentState().globalComposite);
    context->restore();
}

static void drawImageToContext(Image &image, GraphicsContextCG *context, const FloatRect &dest, const FloatRect &src, CompositeOperator op) {
    context->drawImage(&image, dest, src, op);
}

void CanvasRenderingContext2DCG::fullCanvasCompositedDrawImage(Image &image, const FloatRect &dest, const FloatRect &src, CompositeOperator op) {

    IntSize croppedOffset;
    auto bufferRect = calculateCompositingBufferRect(dest, &croppedOffset);
    if (bufferRect.isEmpty()) {
        clearCanvas();
        return;
    }

    auto buffer = createCompositingBuffer(bufferRect);
    if (!buffer)
        return;

    auto c = drawingContext();
    if (!c)
        return;

    FloatRect adjustedDest = dest;
    adjustedDest.origin = FloatPoint(0, 0);
    AffineTransform effectiveTransform = c->getTransform();
    IntRect transformedAdjustedRect = enclosingIntRect(effectiveTransform.mapRect(adjustedDest));

    std::unique_ptr<GraphicsContextCG> context = buffer->createGraphicsContext();
    context->translate(-transformedAdjustedRect.origin.x, -transformedAdjustedRect.origin.y);
    context->translate(croppedOffset.width, croppedOffset.height);
    context->concat(effectiveTransform);
    drawImageToContext(image, context.get(), adjustedDest, src, CompositeSourceOver);

    compositeBuffer(*buffer, bufferRect, op);
    buffer->release();
}

void CanvasRenderingContext2DCG::drawImage(CanvasImageSource *source,
                                           float srcX, float srcY,
                                           float srcWidth, float srcHeight,
                                           float dstX, float dstY,
                                           float dstWidth, float dstHeight) {

    if (!std::isfinite(dstX) || !std::isfinite(dstY) || !std::isfinite(dstWidth) || !std::isfinite(dstHeight)
            || !std::isfinite(srcX) || !std::isfinite(srcY) || !std::isfinite(srcWidth) || !std::isfinite(srcHeight))
        return;

    if (!dstWidth || !dstHeight)
        return;

    if (!srcWidth || !srcHeight)
        return;

    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    FloatRect normalizedSrcRect = normalizeRect({srcX, srcY, srcWidth, srcHeight});
    FloatRect normalizedDstRect = normalizeRect({dstX, dstY, dstWidth, dstHeight});
    FloatRect imageRect;

    scoped_refptr<Image> image;
    if (source->isImageBitmap()) {
        auto bitmap = source->getImage();
        if (!bitmap->imageBuffer() || !bitmap->imageBuffer()->bufferData())
            return;

        imageRect = FloatRect(FloatPoint(), {bitmap->width(), bitmap->height()});
        image = bitmap->imageBuffer()->copyNativeImage();
    } else {
        auto node = source->getNode();
        if (!node) {
            return;
        }

        CanvasRenderingContext2DCG *ctx = dynamic_cast<CanvasRenderingContext2DCG *>(node->getContext2d().get());
        if (!ctx || !ctx->m_imageBuffer) {
            return;
        }

        image = ctx->m_imageBuffer->copyNativeImage();
        imageRect = FloatRect(FloatPoint(), image->size());
    }

    if (!image) {
        return;
    }

    // When the source rectangle is outside the source image, the source rectangle must be clipped
    // to the source image and the destination rectangle must be clipped in the same proportion.
    FloatRect originalNormalizedSrcRect = normalizedSrcRect;
    normalizedSrcRect.intersect(imageRect);
    if (normalizedSrcRect.isEmpty())
        return;

    if (normalizedSrcRect != originalNormalizedSrcRect) {
        normalizedDstRect.setWidth(normalizedDstRect.width() * normalizedSrcRect.width() / originalNormalizedSrcRect.width());
        normalizedDstRect.setHeight(normalizedDstRect.height() * normalizedSrcRect.height() / originalNormalizedSrcRect.height());
        if (normalizedDstRect.isEmpty())
            return;
    }

    CompositeOperator op = currentState().globalComposite;
    BlendMode blendMode = currentState().globalBlend;

    if (rectContainsCanvas(normalizedDstRect)) {
        context->drawImage(image.get(), normalizedDstRect, normalizedSrcRect, op, blendMode);
        didDrawEntireCanvas();
    } else if (isFullCanvasCompositeMode(op)) {
        fullCanvasCompositedDrawImage(*image, normalizedDstRect, normalizedSrcRect, op);
        didDrawEntireCanvas();
    } else if (op == CompositeCopy) {
        clearCanvas();
        context->drawImage(image.get(), normalizedDstRect, normalizedSrcRect, op, blendMode);
        didDrawEntireCanvas();
    } else {
        context->drawImage(image.get(), normalizedDstRect, normalizedSrcRect, op, blendMode);
        didDraw(normalizedDstRect);
    }
}

scoped_refptr<ImageData> CanvasRenderingContext2DCG::getImageData(int sx, int sy, int sw, int sh) {

    if (!std::isfinite(sx) || !std::isfinite(sy) || !std::isfinite(sw) || !std::isfinite(sh)) {
        return nullptr;
    }

    if (!sw || !sh)
        return nullptr;

    if (sw < 0) {
        sx += sw;
        sw = -sw;
    }
    if (sh < 0) {
        sy += sh;
        sh = -sh;
    }

    FloatRect logicalRect(sx, sy, sw, sh);
    if (logicalRect.width() < 1)
        logicalRect.setWidth(1);
    if (logicalRect.height() < 1)
        logicalRect.setHeight(1);

    IntRect imageDataRect = enclosingIntRect(logicalRect);

    if (!m_imageBuffer)
        return ImageData::Create(imageDataRect.size, static_cast<size_t>(4 * imageDataRect.width()));

    return m_imageBuffer->getUnmultipliedImageData(imageDataRect, nullptr);
}

void CanvasRenderingContext2DCG::putImageData(ImageData *data, int x, int y, int dirtyX, int dirtyY, int dirtyWidth, int dirtyHeight) {

    if (!m_imageBuffer)
        return;

    if (!data->data())
        return;

    if (dirtyWidth < 0) {
        dirtyX += dirtyWidth;
        dirtyWidth = -dirtyWidth;
    }

    if (dirtyHeight < 0) {
        dirtyY += dirtyHeight;
        dirtyHeight = -dirtyHeight;
    }

    FloatRect clipRect(dirtyX, dirtyY, dirtyWidth, dirtyHeight);
    clipRect.intersect(IntRect(0, 0, data->width(), data->height()));
    IntSize destOffset(x, y);
    IntRect destRect = enclosingIntRect(clipRect);
    destRect.move(destOffset);
    destRect.intersect(IntRect(IntPoint(), m_imageBuffer->resolutionSize()));
    if (destRect.isEmpty())
        return;
    IntRect sourceRect(destRect);
    sourceRect.move(-destOffset);
    sourceRect.intersect(IntRect(0, 0, data->width(), data->height()));

    if (!sourceRect.isEmpty()) {
        m_imageBuffer->putByteArray(*data, sourceRect, IntPoint(destOffset), ImageBuffer::LogicalCoordinateSystem);
    }

    didDraw(destRect, CanvasDidDrawApplyNone); // ignore transform, shadow and clip
}

void CanvasRenderingContext2DCG::setGlobalAlpha(float alpha) {
    if (!(alpha >= 0 && alpha <= 1))
        return;
    if (currentState().globalAlpha == alpha)
        return;
    realizeSaves();
    currentState().globalAlpha = alpha;

    auto context = drawingContext();
    if (!context)
        return;
    context->setGlobalAlpha(alpha);
}

void CanvasRenderingContext2DCG::setGlobalCompositeOperation(const CompositeOperator operation, const BlendMode blendMode) {
    if ((currentState().globalComposite == operation) && (currentState().globalBlend == blendMode))
        return;
    realizeSaves();
    currentState().globalComposite = operation;
    currentState().globalBlend = blendMode;

    auto context = drawingContext();
    if (!context)
        return;
    context->setCompositeOperation(operation, blendMode);
}

void CanvasRenderingContext2DCG::realizeSaves() {
    if (m_unrealizedSaveCount)
        realizeSavesLoop();
}

static const unsigned MaxSaveCount = 1024 * 16;

void CanvasRenderingContext2DCG::realizeSavesLoop() {
    auto context = drawingContext();

    do {
        if (m_stateStack.size() > MaxSaveCount)
            break;
        m_stateStack.push_back(currentState());
        if (context)
            context->save();
    } while (--m_unrealizedSaveCount);
}

void CanvasRenderingContext2DCG::save() {
    ++m_unrealizedSaveCount;
}

void CanvasRenderingContext2DCG::restore() {
    if (m_unrealizedSaveCount) {
        --m_unrealizedSaveCount;
        return;
    }
    if (m_stateStack.size() <= 1)
        return;
    m_path->transform(currentState().transform);
    m_stateStack.pop_back();
    AffineTransform inverse;
    if (currentState().transform.inverse(inverse))
        m_path->transform(inverse);

    auto context = drawingContext();
    if (!context)
        return;
    context->restore();
}

void CanvasRenderingContext2DCG::createEvent() {

}

void *CanvasRenderingContext2DCG::toDataURL() {
    return nullptr;
}

void CanvasRenderingContext2DCG::enableConsumingDrawCache() {
    m_cacheConsuming = true;
}

void CanvasRenderingContext2DCG::releaseConsumingImage() {
    m_cacheConsuming = false;
    if (m_consumingDrawCachedImage) {
        m_consumingDrawCachedImage.reset();
        m_dirtyRect = FloatRect::ZERO;
    }
}

void CanvasRenderingContext2DCG::paintInContext(GraphicsContext *context, const FloatRect &contentBounds) {
    GraphicsContextCG *ctx = dynamic_cast<GraphicsContextCG *>(context);
    if (!ctx || !m_imageBuffer) {
        return;
    }

    FloatRect srcRect = mapRect(contentBounds, {FloatPoint{}, this->m_node->getFrame().size}, {FloatPoint(), m_imageBuffer->resolutionSize()});

    if (m_cacheConsuming) {
        if (!m_consumingDrawCachedImage) {
            m_consumingDrawCachedImage = m_imageBuffer->copyNativeImage();
        }

        ctx->drawImage(m_consumingDrawCachedImage.get(), contentBounds, srcRect, CompositeCopy, BlendMode::Normal);
    } else {
        scoped_refptr<Image> image = m_imageBuffer->copyNativeImage();
        ctx->drawImage(image.get(), contentBounds, srcRect, CompositeCopy, BlendMode::Normal);
        m_dirtyRect = FloatRect::ZERO;
    }
}

GraphicsContextCG *CanvasRenderingContext2DCG::drawingContext() {
    if (!m_drawingContext) {
        updateDrawingContext();
    }

    return m_drawingContext.get();
}

void CanvasRenderingContext2DCG::updateDrawingContext() {
    FloatSize contentSize = m_node->contentSize();

    m_imageBuffer = ImageBuffer::CreateScopedRef(expandedIntSize(contentSize), 4);
    m_drawingContext = std::move(m_imageBuffer->createGraphicsContext());

    m_drawingContext->setImageInterpolationQuality(InterpolationLow);
    m_drawingContext->setStrokeThickness(1);
}

void CanvasRenderingContext2DCG::ensurePlatformPath() {
    if (!m_path) {
        m_path.reset(new Path());
    }
}

void CanvasRenderingContext2DCG::clearCanvas() {
    auto context = drawingContext();
    if (!context)
        return;

    context->save();
    context->setTransform(m_imageBuffer->baseTransform());
    context->clearRect({{0, 0}, contentSize()});
    context->restore();
}

void CanvasRenderingContext2DCG::didDrawEntireCanvas() {
    didDraw(FloatRect(FloatPoint(), m_imageBuffer->size()), CanvasDidDrawApplyClip);
}

void CanvasRenderingContext2DCG::didDraw(const FloatRect &r, unsigned options) {
    auto context = drawingContext();
    if (!context)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    FloatRect dirtyRect = r;
    if (options & CanvasDidDrawApplyTransform) {
        AffineTransform ctm = currentState().transform;
        dirtyRect = ctm.mapRect(r);
    }

    if (options & CanvasDidDrawApplyShadow && currentState().shadowColor.isVisible()) {
        // The shadow gets applied after transformation
        FloatRect shadowRect(dirtyRect);
        auto &offset = currentState().shadowOffset;
        shadowRect.move(offset.width, offset.height);
        shadowRect.inflate(currentState().shadowBlur);
        dirtyRect.unite(shadowRect);
    }

    if (options & CanvasDidDrawApplyClip) {
        // FIXME: apply the current clip to the rectangle. Unfortunately we can't get the clip
        // back out of the GraphicsContext, so to take clip into account for incremental painting,
        // we'd have to keep the clip path around.
    }

    FloatRect destRect = {{0, 0}, m_node->getFrame().size};

    // Inflate dirty rect to cover antialiasing on image buffers.
    dirtyRect.inflate(1);

    FloatRect mappedRect = mapRect(dirtyRect, FloatRect({0, 0}, contentSize()), destRect);
    mappedRect.intersect(destRect);

    if (!r.isEmpty() && !m_dirtyRect.containsRect(mappedRect)) {
        m_dirtyRect.unite(mappedRect);
        if (m_node->graphicsLayer()) {
            m_node->graphicsLayer()->getPlatformLayer()->setNeedsDisplayInRect(m_dirtyRect);
        }
    }
}

CanvasRenderingContext2DState &CanvasRenderingContext2DCG::currentState() {
    return m_stateStack.back();
}

void CanvasRenderingContext2DCG::reset() {
    m_stateStack.clear();
    m_stateStack.push_back(CanvasRenderingContext2DState());
    m_path->clear();
    m_drawingContext.reset();
}

}