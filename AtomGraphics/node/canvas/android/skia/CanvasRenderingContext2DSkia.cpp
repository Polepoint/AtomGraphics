//
// Created by neo on 2018/9/1.
//

#include "gpu/GLResourceManager.h"
#include "javascript/android/JSPageScope.h"
#include "javascript/android/JSPageScopeTracer.h"
#include "memory/ScopedRefPtr.h"
#include "graphics/android/skia/SkiaUtils.h"
#include "graphics/android/skia/GraphicsContextSaverSkia.h"
#include "graphics/GraphicsLayer.h"
#include "node/canvas/CanvasRenderingContext2DState.h"
#include "node/canvas/CSSParser.h"
#include "CanvasRenderingContext2DSkia.h"

namespace AtomGraphics {

const long kDrawImageTextureUploadSoftSizeLimit = 4096 * 4096 / 2;
const long kDrawImageTextureUploadHardSizeLimit = 8192 * 8192 / 2;
const long kDrawImageTextureUploadSoftSizeLimitScaleThreshold = 4;

const int kGPUReadbackMinSuccessiveFrames = 3;

CanvasRenderingContext2DSkia::CanvasRenderingContext2DSkia(CanvasNode *node) : CanvasRenderingContext2D(node) {
    JS::JSPageScope *scope = JS::JSPageScopeTracer::CurrentPageScope();
    if (scope) {
        m_hostPageID = scope->getPageID();
    }
    m_stateStack.push_back(CanvasRenderingContext2DState());
    m_path = std::unique_ptr<Path>(new Path());
    if (!contentSize().isEmpty()) {
        StartRecording();
    }
}

void CanvasRenderingContext2DSkia::setFillStyle(const CanvasStyle &style) {
    if (currentState().fillStyle == style) {
        return;
    }

    modifiableState().fillStyle = style;
    auto context = drawingContext();
    if (context) {
        context->setFillStyle(style);
    }
}

void CanvasRenderingContext2DSkia::setStrokeStyle(const CanvasStyle &style) {
    if (currentState().strokeStyle == style) {
        return;
    }

    modifiableState().strokeStyle = style;
    auto context = drawingContext();
    if (context) {
        context->setStrokeStyle(style);
    }
}

void CanvasRenderingContext2DSkia::setShadowColor(const Color4F &color) {
    if (currentState().shadowColor == color) {
        return;
    }
    modifiableState().shadowColor = color;
}

void CanvasRenderingContext2DSkia::setShadowBlur(float blur) {
    if (!std::isfinite(blur) || blur < 0)
        return;

    if (currentState().shadowBlur == blur) {
        return;
    }
    modifiableState().shadowBlur = blur;
}

void CanvasRenderingContext2DSkia::setShadowOffsetX(float offsetX) {
    if (!std::isfinite(offsetX))
        return;

    if (currentState().shadowOffset.width == offsetX) {
        return;
    }
    modifiableState().shadowOffset.width = offsetX;
}

void CanvasRenderingContext2DSkia::setShadowOffsetY(float offsetY) {
    if (!std::isfinite(offsetY))
        return;

    if (currentState().shadowOffset.height == offsetY) {
        return;
    }
    modifiableState().shadowOffset.height = offsetY;
}


void CanvasRenderingContext2DSkia::setLineCap(const LineCap lineCap) {
    if (currentState().lineCap == lineCap) {
        return;
    }

    modifiableState().lineCap = lineCap;

    auto context = drawingContext();
    if (!context)
        return;
    context->setLineCap(lineCap);
}

void CanvasRenderingContext2DSkia::setLineJoin(const LineJoin lineJoin) {
    if (currentState().lineJoin == lineJoin) {
        return;
    }

    modifiableState().lineJoin = lineJoin;

    auto context = drawingContext();
    if (!context)
        return;
    context->setLineJoin(lineJoin);
}

void CanvasRenderingContext2DSkia::setLineWidth(float width) {
    if (!(std::isfinite(width) && width > 0))
        return;

    if (currentState().lineWidth == width) {
        return;
    }

    modifiableState().lineWidth = width;

    auto context = drawingContext();
    if (!context) {
        return;
    }
    context->setStrokeThickness(width);
}

void CanvasRenderingContext2DSkia::setMiterLimit(float limit) {
    if (!(std::isfinite(limit) && limit > 0))
        return;

    if (currentState().miterLimit == limit) {
        return;
    }
    modifiableState().miterLimit = limit;

    auto context = drawingContext();
    if (!context) {
        return;
    }

    context->setMiterLimit(limit);
}

static bool lineDashSequenceIsValid(const std::vector<float> &dash) {
    for (size_t i = 0; i < dash.size(); i++) {
        if (!std::isfinite(dash[i]) || dash[i] < 0)
            return false;
    }
    return true;
}

void CanvasRenderingContext2DSkia::setLineDash(const std::vector<float> &dash) {
    if (!lineDashSequenceIsValid(dash))
        return;

    modifiableState().lineDash = dash;
    applyLineDash();
}

void CanvasRenderingContext2DSkia::setLineDashOffset(float offset) {
    if (!std::isfinite(offset) || currentState().lineDashOffset == offset)
        return;

    modifiableState().lineDashOffset = offset;
    applyLineDash();
}

void CanvasRenderingContext2DSkia::applyLineDash() const {
    GraphicsContext *c = drawingContext();
    if (!c)
        return;
    DashArray convertedLineDash(currentState().lineDash.size());
    for (size_t i = 0; i < currentState().lineDash.size(); ++i)
        convertedLineDash[i] = static_cast<DashArrayElement>(currentState().lineDash[i]);
    c->setLineDash(convertedLineDash, currentState().lineDashOffset);
}

void CanvasRenderingContext2DSkia::rect(float x, float y, float width, float height) {
    if (!currentState().hasInvertibleTransform)
        return;

    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(width) ||
            !std::isfinite(height))
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

void CanvasRenderingContext2DSkia::fillRect(float x, float y, float width, float height) {
    if (!validateRectForCanvas(x, y, width, height))
        return;

    auto ctx = drawingContext();
    if (!ctx)
        return;
    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    FloatRect dst_rect(x, y, width, height);

    Draw([dst_rect](GraphicsContextSkia *c) {
             c->fillRect(dst_rect);
         },
         [&dst_rect](const IntRect &clip_bounds) {   // overdraw test lambda
             return dst_rect.containsRect(clip_bounds);
         },
         dst_rect,
         currentState().fillStyle);
}

void CanvasRenderingContext2DSkia::strokeRect(float x, float y, float width, float height) {
    if (!validateRectForCanvas(x, y, width, height))
        return;

    auto ctx = drawingContext();
    if (!ctx)
        return;
    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    FloatRect dst_rect(x, y, width, height);

    float lineWidth = currentState().lineWidth;

    Draw([dst_rect, lineWidth](GraphicsContextSkia *c) {
             c->strokeRect(dst_rect, lineWidth);
         },
         [&dst_rect](const IntRect &clip_bounds) {   // overdraw test lambda
             return dst_rect.containsRect(clip_bounds);
         },
         dst_rect,
         currentState().strokeStyle);
}

void CanvasRenderingContext2DSkia::clearRect(float x, float y, float width, float height) {
    if (!validateRectForCanvas(x, y, width, height))
        return;
    auto ctx = drawingContext();
    if (!ctx)
        return;
    if (!currentState().hasInvertibleTransform)
        return;

    FloatRect rect(x, y, width, height);
    ctx->clearRect(rect);
    DidDraw(rect);
}

void CanvasRenderingContext2DSkia::fill(WindRule windRule) {
    DrawPathInternal(*m_path.get(),
                     CanvasRenderingContext2DState::PaintType::kFillPaintType,
                     WebCoreWindingRuleToSkiaFillType(windRule));
}

void CanvasRenderingContext2DSkia::stroke() {
    DrawPathInternal(*m_path.get(),
                     CanvasRenderingContext2DState::PaintType::kStrokePaintType);
}

void CanvasRenderingContext2DSkia::InflateStrokeRect(FloatRect &rect) const {
    // Fast approximation of the stroke's bounding rect.
    // This yields a slightly oversized rect but is very fast
    // compared to Path::strokeBoundingRect().
    static const double kRoot2 = sqrtf(2);
    float delta = currentState().lineWidth / 2;
    if (currentState().lineJoin == MiterJoin)
        delta *= currentState().miterLimit;
    else if (currentState().lineCap == SquareCap)
        delta *= kRoot2;

    rect.inflate(delta);
}


void CanvasRenderingContext2DSkia::DrawPathInternal(
        Path &path,
        CanvasRenderingContext2DState::PaintType paint_type,
        SkPath::FillType fill_type) {
    if (path.isEmpty())
        return;

    FloatRect bounds = path.boundingRect();
    if (isnan(bounds.x()) || isnan(bounds.y()) || isnan(bounds.width()) ||
            isnan(bounds.height()))
        return;
    path.setFillType(fill_type);

    if (paint_type == CanvasRenderingContext2DState::kStrokePaintType)
        InflateStrokeRect(bounds);

    if (!drawingContext())
        return;

    const CanvasStyle &style = paint_type == CanvasRenderingContext2DState::kFillPaintType ? currentState().fillStyle
                                                                                           : currentState().strokeStyle;

    Draw([&path, paint_type](GraphicsContextSkia *c) {  // draw lambda
             if (paint_type == CanvasRenderingContext2DState::kFillPaintType) {
                 c->fillPath(path);
             } else {
                 c->strokePath(path);
             }
         },
         [](const SkIRect &rect)  // overdraw test lambda
         { return false; },
         bounds, style);
}

void CanvasRenderingContext2DSkia::beginPath() {
    m_path->clear();
}

void CanvasRenderingContext2DSkia::moveTo(float x, float y) {
    m_path->moveTo(x, y);
}

void CanvasRenderingContext2DSkia::closePath() {
    if (m_path->isEmpty())
        return;
    FloatRect bound = m_path->boundingRect();
    if (bound.size.width || bound.size.height)
        m_path->closePath();
}

void CanvasRenderingContext2DSkia::lineTo(float x, float y) {
    if (!std::isfinite(x) || !std::isfinite(y))
        return;
    FloatPoint p1 = FloatPoint(x, y);

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform) {
        p1 = curState.transform.mapPoint(p1);
    }

    if (!m_path->hasCurrentPoint())
        m_path->moveTo(p1.x, p1.y);

    m_path->addLineTo(x, y);
}

void CanvasRenderingContext2DSkia::clip(WindRule windRule) {
    auto ctx = drawingContext();
    if (!ctx)
        return;
    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    modifiableState().ClipPath(m_path->getSkPath());
    if (m_path->isEmpty()) {
        ctx->clipRect(FloatRect::ZERO);
    } else {
        ctx->clipPath(m_path.get(), windRule);
    }
}

void CanvasRenderingContext2DSkia::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    if (!std::isfinite(cpx) || !std::isfinite(cpy) || !std::isfinite(x) || !std::isfinite(y))
        return;
    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;
    if (m_path->isEmpty()) {
        m_path->moveTo(cpx, cpy);
    }

    FloatPoint currentPoint = m_path->currentPoint();
    FloatPoint p1 = FloatPoint(x, y);
    FloatPoint cp = FloatPoint(cpx, cpy);
    if (!p1.equals(currentPoint) || !p1.equals(cp)) {
        m_path->addQuadCurveTo(cpx, cpy, x, y);
    }
}

void CanvasRenderingContext2DSkia::bezierCurveTo(float cp1x, float cp1y,
                                                 float cp2x, float cp2y,
                                                 float x, float y) {
    if (!std::isfinite(cp1x) || !std::isfinite(cp1y) || !std::isfinite(cp2x) ||
            !std::isfinite(cp2y) || !std::isfinite(x) || !std::isfinite(y))
        return;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;
    if (!m_path->hasCurrentPoint()) {
        m_path->moveTo(cp1x, cp1y);
    }

    FloatPoint p1 = FloatPoint(x, y);
    FloatPoint cp1 = FloatPoint(cp1x, cp1y);
    FloatPoint cp2 = FloatPoint(cp2x, cp2y);
    FloatPoint currentPoint = m_path->currentPoint();
    if (!p1.equals(currentPoint) || !p1.equals(cp1) || !p1.equals(cp2)) {
        m_path->addBezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
    }

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


void CanvasRenderingContext2DSkia::arc(float x, float y, float radius, float startAngle, float endAngle,
                                       bool counterclockwise) {

    if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(radius) ||
            !std::isfinite(startAngle) || !std::isfinite(endAngle))
        return;

    if (radius < 0)
        return;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    normalizeAngles(startAngle, endAngle, counterclockwise);

    if (!radius || startAngle == endAngle) {
        lineTo(x + radius * cosf(startAngle), y + radius * sinf(startAngle));
        return;
    }

    m_path->addArc(x, y, radius, startAngle, endAngle, counterclockwise);

}

void CanvasRenderingContext2DSkia::arcTo(float x1, float y1, float x2, float y2, float r) {
    m_path->addArcToPoint(x1, y1, x2, y2, r);

}

bool CanvasRenderingContext2DSkia::isPointInPath(float x, float y) {
    auto c = drawingContext();
    if (!c)
        return false;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return false;

    FloatPoint transformedPoint;

    AffineTransform ctm;
    if (curState.transform.inverse(ctm)) {
        transformedPoint = ctm.mapPoint(x, y);
    } else {
        transformedPoint = FloatPoint(x, y);
    }

    if (!std::isfinite(transformedPoint.x) || !std::isfinite(transformedPoint.y))
        return false;

    return m_path->containsPoint(x, y);
}

void CanvasRenderingContext2DSkia::scale(float scaleWidth, float scaleHeight) {
    auto c = drawingContext();
    if (!c)
        return;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    if (!std::isfinite(scaleWidth) || !std::isfinite(scaleHeight))
        return;

    AffineTransform newTransform = curState.transform;
    newTransform.scale(scaleWidth, scaleHeight);
    if (curState.transform == newTransform)
        return;

    if (!scaleWidth || !scaleHeight) {
        modifiableState().hasInvertibleTransform = false;
        return;
    }

    modifiableState().transform = newTransform;

    c->scale(scaleWidth, scaleHeight);
    m_path->transform(AffineTransform().scale(1.0 / scaleWidth, 1.0 / scaleHeight));
}

void CanvasRenderingContext2DSkia::rotate(float angleInRadians) {
    auto c = drawingContext();
    if (!c)
        return;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    if (!std::isfinite(angleInRadians))
        return;

    AffineTransform newTransform = curState.transform;
    newTransform.rotate(angleInRadians / M_PI * 180.0);

    if (curState.transform == newTransform)
        return;

    modifiableState().transform = newTransform;

    c->rotate(angleInRadians);
    m_path->transform(AffineTransform().rotate(-angleInRadians / M_PI * 180.0));
}

void CanvasRenderingContext2DSkia::translate(float tx, float ty) {
    auto c = drawingContext();
    if (!c)
        return;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    if (!std::isfinite(tx) | !std::isfinite(ty))
        return;

    AffineTransform newTransform = curState.transform;
    newTransform.translate(tx, ty);
    if (curState.transform == newTransform)
        return;

    modifiableState().transform = newTransform;
    c->translate(tx, ty);
    m_path->transform(AffineTransform().translate(-tx, -ty));
}

void CanvasRenderingContext2DSkia::transform(float a, float b, float c, float d, float tx, float ty) {
    auto ctx = drawingContext();
    if (!ctx)
        return;

    auto &curState = currentState();
    if (!curState.hasInvertibleTransform)
        return;

    if (!std::isfinite(a) | !std::isfinite(c) | !std::isfinite(tx)
            | !std::isfinite(b) | !std::isfinite(d) | !std::isfinite(ty))
        return;

    AffineTransform transform(a, b, c, d, tx, ty);
    AffineTransform newTransform = curState.transform * transform;
    if (curState.transform == newTransform)
        return;

    AffineTransform inverseValue;
    if (transform.inverse(inverseValue)) {
        modifiableState().transform = newTransform;
        ctx->concat(transform);
        if (!inverseValue.isIdentity() && !m_path->isEmpty()) {
            m_path->transform(inverseValue);
        }
        return;
    }

    modifiableState().hasInvertibleTransform = false;
}

void CanvasRenderingContext2DSkia::setTransform(float a, float b, float c, float d, float tx, float ty) {
    auto ctx = drawingContext();
    if (!ctx)
        return;

    if (!std::isfinite(a) | !std::isfinite(c) | !std::isfinite(tx) | !std::isfinite(b) |
            !std::isfinite(d) | !std::isfinite(ty))
        return;

    auto &curState = currentState();

    AffineTransform ctm = curState.transform;
    bool hasInvertibleTransform = curState.hasInvertibleTransform;

    ctx->setTransform(AffineTransform());
    modifiableState().transform = AffineTransform();

    if (hasInvertibleTransform) {
        if (!ctm.isIdentity() && !m_path->isEmpty()) {
            m_path->transform(ctm);
        }
    }
    modifiableState().hasInvertibleTransform = true;

    transform(a, b, c, d, tx, ty);
}

void CanvasRenderingContext2DSkia::setFont(const std::string &fontString) {
    auto &curState = currentState();
    if (curState.unparsedFont == fontString) {
        return;
    }

    Font font;
    FontDescription description;
    if (!CSSParser::parseCSSString(fontString, description)) {
        return;
    }

    font.setFontDescription(description);
    modifiableState().unparsedFont = fontString;
    modifiableState().font = font;
}

std::string CanvasRenderingContext2DSkia::getFont() {
    auto &curState = currentState();
    if (!curState.unparsedFont.empty()) {
        return curState.unparsedFont;
    }

    return CSSParser::fontToCSSConfig(modifiableState().font);
}

void CanvasRenderingContext2DSkia::setTextAlign(TextAlign textAlign) {
    auto &curState = currentState();
    if (curState.textAlign == textAlign) {
        return;
    }

    modifiableState().textAlign = textAlign;
}

void CanvasRenderingContext2DSkia::setTextBaseline(TextBaseline textBaseline) {
    auto &curState = currentState();
    if (curState.textBaseline == textBaseline) {
        return;
    }

    modifiableState().textBaseline = textBaseline;
}

void CanvasRenderingContext2DSkia::fillText(const base::String &text, float x, float y, float *maxWidth) {
    DrawTextInternal(text, x, y, CanvasRenderingContext2DState::kFillPaintType, maxWidth);
}

void CanvasRenderingContext2DSkia::strokeText(const base::String &text, float x, float y, float *maxWidth) {
    DrawTextInternal(text, x, y, CanvasRenderingContext2DState::kStrokePaintType, maxWidth);
}

void CanvasRenderingContext2DSkia::DrawTextInternal(const base::String &text, float x, float y,
                                                    CanvasRenderingContext2DState::PaintType paint_type,
                                                    float *max_width) {
    // The style resolution required for fonts is not available in frame-less
    // documents.
    if (contentSize().isEmpty())
        return;

    auto c = drawingContext();
    if (!c)
        return;

    if (!std::isfinite(x) || !std::isfinite(y))
        return;
    if (max_width && (!std::isfinite(*max_width) || *max_width <= 0))
        return;
    if (!text.length()) {
        return;
    }

    // Currently, SkPictureImageFilter does not support subpixel text
    // anti-aliasing, which is expected when !creationAttributes().alpha, so we
    // need to fall out of display list mode when drawing text to an opaque
    // canvas. crbug.com/583809
//    if (!IsAccelerated()) {
//        canvas()->DisableDeferral(
//                kDisableDeferralReasonSubPixelTextAntiAliasingSupport);
//    }

    const Font &font = currentState().font;
    const FontMetrics &font_metrics = font.PrimaryFontData()->fontMetrics();
    // FIXME: Need to turn off font smoothing.

    TextDirection direction = currentState().textDirection;
    bool is_rtl = direction == TextDirection::RTL;
    // Draw the item text at the correct point.
    FloatPoint location(x, y);
    float text_width = font.widthForString(text);
    location += textOffset(text_width, font_metrics, currentState().textDirection);

    bool use_max_width = (max_width && *max_width < text_width);
    float width = use_max_width ? *max_width : text_width;

    FloatRect bounds = FloatRect(location.x - font_metrics.height() / 2,
                                 location.y - font_metrics.ascent() - font_metrics.lineGap(),
                                 width + font_metrics.height(),
                                 font_metrics.lineSpacing());

    if (paint_type == CanvasRenderingContext2DState::kStrokePaintType)
        InflateStrokeRect(bounds);

    GraphicsContextSaverSkia state_restorer(c);
    if (use_max_width) {
        c->save();
        c->translate(location.x, location.y);
        // We draw when fontWidth is 0 so compositing operations (eg, a "copy" op)
        // still work.
        c->scale((text_width > 0 ? width / text_width : 0), 1);
        location = FloatPoint();
    }

    const CanvasStyle &style = paint_type == CanvasRenderingContext2DState::kFillPaintType ? currentState().fillStyle
                                                                                           : currentState().strokeStyle;
    Draw([&text, &font, &location, paint_type](GraphicsContextSkia *c) {
             c->drawText(text, location.x, location.y, font, paint_type == CanvasRenderingContext2DState::kFillPaintType);
         },
         [](const SkIRect &rect)  // overdraw test lambda
         { return false; },
         bounds,
         style);
}

FloatPoint CanvasRenderingContext2DSkia::textOffset(
        float width,
        const FontMetrics &fontMetrics,
        TextDirection direction) const {

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


TextMetrics CanvasRenderingContext2DSkia::measureText(const base::String &text) {
    SkScalar width = currentState().font.widthForString(text);
    TextMetrics textMetrics = TextMetrics();
    textMetrics.setWidth(SkScalarToFloat(width));
    return textMetrics;
}


static inline FloatRect NormalizeRect(const FloatRect &rect) {
    return FloatRect(std::min(rect.x(), rect.getMaxX()),
                     std::min(rect.y(), rect.getMaxY()),
                     std::max(rect.width(), -rect.width()),
                     std::max(rect.height(), -rect.height()));
}

static inline void ClipRectsToImageRect(const FloatRect &image_rect,
                                        FloatRect *src_rect,
                                        FloatRect *dst_rect) {
    if (image_rect.containsRect(*src_rect))
        return;

    // Compute the src to dst transform
    FloatSize scale(dst_rect->size.width / src_rect->size.width,
                    dst_rect->size.height / src_rect->size.height);
    FloatPoint scaled_src_location = src_rect->origin;
    scaled_src_location.scale(scale.width, scale.height);
    FloatSize offset = dst_rect->origin - scaled_src_location;

    src_rect->intersect(image_rect);

    // To clip the destination rectangle in the same proportion, transform the
    // clipped src rect
    *dst_rect = *src_rect;
    dst_rect->scale(scale.width, scale.height);
    dst_rect->move(offset);
}

void CanvasRenderingContext2DSkia::drawImage(CanvasImageSource *image_source,
                                             float sx, float sy, float sWidth, float sHeight,
                                             float dx, float dy, float dWidth, float dHeight) {
    if (!drawingContext())
        return;

    scoped_refptr<Image> image;
    FloatSize default_object_size(contentSize());
    AccelerationHint hint = IsAccelerated() ? kPreferAcceleration : kPreferNoAcceleration;
    image = image_source->GetSourceImageForCanvas(hint);
    if (!image || !image->width() || !image->height())
        return;

    if (!std::isfinite(dx) || !std::isfinite(dy) || !std::isfinite(dWidth) ||
            !std::isfinite(dHeight) || !std::isfinite(sx) || !std::isfinite(sy) ||
            !std::isfinite(sWidth) || !std::isfinite(sHeight) || !dWidth || !dHeight || !sWidth || !sHeight)
        return;


    FloatRect src_rect = NormalizeRect(FloatRect(sx, sy, sWidth, sHeight));
    FloatRect dst_rect = NormalizeRect(FloatRect(dx, dy, dWidth, dHeight));
    FloatSize image_size = image_source->size();

    ClipRectsToImageRect(FloatRect(FloatPoint(), image_size), &src_rect,
                         &dst_rect);

    if (src_rect.isEmpty())
        return;


    // Heuristic for disabling acceleration based on anticipated texture upload
    // overhead.
    // See comments in CanvasHeuristicParameters.h for explanation.
    if (IsAccelerated() && !image_source->IsAccelerated()) {
        float src_area = src_rect.width() * src_rect.height();
        if (src_area > kDrawImageTextureUploadHardSizeLimit) {
            this->DisableAcceleration();
        } else if (src_area > kDrawImageTextureUploadSoftSizeLimit) {
            SkRect bounds = dst_rect;
            AffineTransform ctm = drawingContext()->getTransform();
            bounds = ctm.mapRect(bounds);
            float dst_area = dst_rect.width() * dst_rect.height();
            if (src_area > dst_area * kDrawImageTextureUploadSoftSizeLimitScaleThreshold) {
                this->DisableAcceleration();
            }
        }
    }

    Draw([this, &image, &src_rect, dst_rect](GraphicsContextSkia *c) {  // draw lambda
             auto &state = currentState();
             DrawImageInternal(c, image.get(),
                               src_rect, dst_rect,
                               state.globalComposite, state.globalBlend);
         },
         [this, &dst_rect](const IntRect &clip_bounds) {   // overdraw test lambda
             return RectContainsTransformedRect(dst_rect, clip_bounds);
         },
         dst_rect,
         currentState().fillStyle,
         image_source->IsOpaque() ? kOpaqueImage : kNonOpaqueImage);

}

bool CanvasRenderingContext2DSkia::RectContainsTransformedRect(
        const FloatRect &rect,
        const IntRect &transformed_rect) const {
    FloatQuad quad(rect);
    FloatQuad transformed_quad(transformed_rect);

    return currentState().transform.mapQuad(quad).containsQuad(transformed_quad);
}

void CanvasRenderingContext2DSkia::DrawImageInternal(GraphicsContextSkia *c, Image *image,
                                                     const FloatRect &src_rect,
                                                     const FloatRect &dst_rect,
                                                     CompositeOperator compositeOperator,
                                                     BlendMode blendMode) {
    int initial_save_count = c->getSkCanvas()->getSaveCount();

    bool savedShouldAntialias = c->ShouldAntialias();
    c->SetShouldAntialias(ShouldDrawImageAntialiased(dst_rect));
    c->drawImage(image, dst_rect, src_rect, compositeOperator, blendMode);
    c->SetShouldAntialias(savedShouldAntialias);

    c->getSkCanvas()->restoreToCount(initial_save_count);
}


bool CanvasRenderingContext2DSkia::ShouldDrawImageAntialiased(
        const FloatRect &dest_rect) const {

//    if (IsAccelerated()) {
//        // Don't enable anti-aliasing on accelerated skia renderer
//        return false;
//    }

    auto c = drawingContext();

    const SkMatrix &ctm = c->getTransform();
    // Don't disable anti-aliasing if we're rotated or skewed.
    if (!ctm.rectStaysRect())
        return true;
    // Check if the dimensions of the destination are "small" (less than one
    // device pixel). To prevent sudden drop-outs. Since we know that
    // kRectStaysRect_Mask is set, the matrix either has scale and no skew or
    // vice versa. We can query the kAffine_Mask flag to determine which case
    // it is.
    // FIXME: This queries the CTM while drawing, which is generally
    // discouraged. Always drawing with AA can negatively impact performance
    // though - that's why it's not always on.
    SkScalar width_expansion, height_expansion;
    if (ctm.getType() & SkMatrix::kAffine_Mask) {
        width_expansion = ctm[SkMatrix::kMSkewY];
        height_expansion = ctm[SkMatrix::kMSkewX];
    } else {
        width_expansion = ctm[SkMatrix::kMScaleX];
        height_expansion = ctm[SkMatrix::kMScaleY];
    }
    return dest_rect.width() * fabs(width_expansion) < 1 ||
            dest_rect.height() * fabs(height_expansion) < 1;
}


scoped_refptr<ImageData> CanvasRenderingContext2DSkia::getImageData(
        int sx,
        int sy,
        int sw,
        int sh) {

    if (!std::isfinite(sx) || !std::isfinite(sy) || !std::isfinite(sw) || !std::isfinite(sh)) {
        return nullptr;
    }

    if (!sw || !sh || sw < 0 || sh < 0) {
        return nullptr;
    }

    IntRect image_data_rect(sx, sy, sw, sh);

    sk_sp<SkImage> snapshot = GetImage(kPreferNoAcceleration);

    if (IsAccelerated() && snapshot) {
        gpu_readback_invoked_in_current_frame_ = true;
    }

    SkImageInfo info = SkImageInfo::Make(
            sw,
            sh,
            kRGBA_8888_SkColorType,
            kUnpremul_SkAlphaType,
            SkColorSpace::MakeSRGB());

    void *data = malloc(info.computeMinByteSize());
    snapshot->readPixels(info, data, info.minRowBytes(), sx, sy);
    return ImageData::Create(image_data_rect.size, info.minRowBytes(), data, true);
}

sk_sp<SkImage> CanvasRenderingContext2DSkia::GetImage(AccelerationHint hint) {
    sk_sp<SkImage> img;
    if (have_recorded_draw_commands_ && GetOrCreateResourceProvider(hint)) {
        FlushRecording();
    }

    if (m_resourceProvider) {
        img = m_resourceProvider->makeImageSnapshot(hint);
    }

    return img;
}

bool CanvasRenderingContext2DSkia::IsAccelerated() const {
    if (m_resourceProvider) {
        return m_resourceProvider->isAccelerated();
    }

    return true;
}

void CanvasRenderingContext2DSkia::putImageData(ImageData *data,
                                                int dx,
                                                int dy,
                                                int dirty_x,
                                                int dirty_y,
                                                int dirty_width,
                                                int dirty_height) {

    if (dirty_width < 0) {
        dirty_x += dirty_width;
        dirty_width = -dirty_width;
    }

    if (dirty_height < 0) {
        dirty_y += dirty_height;
        dirty_height = -dirty_height;
    }

    if (!drawingContext()) {
        return;
    }

    IntRect dest_rect(dirty_x, dirty_y, dirty_width, dirty_height);
    dest_rect.intersect(IntRect(0, 0, data->width(), data->height()));
    IntSize dest_offset(dx, dy);
    dest_rect.move(dest_offset);
    dest_rect.intersect(IntRect(0, 0, static_cast<int>(contentSize().width), static_cast<int>(contentSize().height)));
    if (dest_rect.isEmpty())
        return;

    IntRect source_rect(dest_rect);
    source_rect.move(-dest_offset);

    CheckOverdraw(dest_rect, ImageType::kNoImage, kUntransformedUnclippedFill);

    PutByteArray(data->data(),
                 IntSize(data->width(), data->height()),
                 source_rect,
                 IntPoint(dest_offset));

    DidDraw(dest_rect);
}

void CanvasRenderingContext2DSkia::PutByteArray(const void *source,
                                                const IntSize &source_size,
                                                const IntRect &source_rect,
                                                const IntPoint &dest_point) {

    size_t bytes_per_pixel = SkColorTypeBytesPerPixel(kRGBA_8888_SkColorType);

    int origin_x = source_rect.x();
    int dest_x = dest_point.x + source_rect.x();

    int origin_y = source_rect.y();
    int dest_y = dest_point.y + source_rect.y();

    const size_t src_bytes_per_row = bytes_per_pixel * source_size.width;
    const void *src_addr = (const unsigned char *) source + origin_y * src_bytes_per_row + origin_x * bytes_per_pixel;

    SkImageInfo info = SkImageInfo::Make(source_rect.width(),
                                         source_rect.height(),
                                         kRGBA_8888_SkColorType,
                                         kUnpremul_SkAlphaType,
                                         SkColorSpace::MakeSRGB());

    WritePixels(info, src_addr, src_bytes_per_row, dest_x, dest_y);
}

bool CanvasRenderingContext2DSkia::WritePixels(const SkImageInfo &orig_info,
                                               const void *pixels,
                                               size_t row_bytes,
                                               int x,
                                               int y) {
    auto resourceProvider = GetOrCreateResourceProvider();
    if (resourceProvider) {
        FlushRecording();
        return resourceProvider->writePixels(orig_info, pixels, row_bytes, x, y);
    }

    return false;
}

void CanvasRenderingContext2DSkia::setGlobalAlpha(float alpha) {
    if (!(alpha >= 0 && alpha <= 1))
        return;
    if (currentState().globalAlpha == alpha)
        return;
    modifiableState().globalAlpha = alpha;
    auto c = drawingContext();
    if (!c)
        return;
    c->setGlobalAlpha(alpha);
}

void CanvasRenderingContext2DSkia::setGlobalCompositeOperation(const CompositeOperator operation,
                                                               const BlendMode blendMode) {
    auto &curState = currentState();
    if ((curState.globalComposite == operation) && (curState.globalBlend == blendMode))
        return;
    modifiableState().globalComposite = operation;
    modifiableState().globalBlend = blendMode;
    auto c = drawingContext();
    if (!c)
        return;

    c->setCompositeOperation(operation, blendMode);
}

void CanvasRenderingContext2DSkia::realizeSaves() {
    if (m_unrealizedSaveCount)
        realizeSavesLoop();
}

static const unsigned MaxSaveCount = 1024 * 16;

void CanvasRenderingContext2DSkia::realizeSavesLoop() {
    auto context = drawingContext();

    do {
        if (m_stateStack.size() > MaxSaveCount)
            break;
        m_stateStack.push_back(currentState());
        if (context)
            context->save();
    } while (--m_unrealizedSaveCount);
}

void CanvasRenderingContext2DSkia::save() {
    ++m_unrealizedSaveCount;
}

void CanvasRenderingContext2DSkia::restore() {
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

void CanvasRenderingContext2DSkia::createEvent() {
    CanvasRenderingContext2D::createEvent();
}

void *CanvasRenderingContext2DSkia::toDataURL() {
    return CanvasRenderingContext2D::toDataURL();
}

const CanvasRenderingContext2DState &CanvasRenderingContext2DSkia::currentState() const {
    return m_stateStack.back();
}

CanvasRenderingContext2DState &CanvasRenderingContext2DSkia::modifiableState() {
    realizeSaves();
    return m_stateStack.back();
}

bool CanvasRenderingContext2DSkia::IsFullCanvasCompositeMode(CompositeOperator op) {
    // See 4.8.11.1.3 Compositing
    // CompositeSourceAtop and CompositeDestinationOut are not listed here as the
    // platforms already implement the specification's behavior.
    return op == CompositeSourceIn || op == CompositeSourceOut ||
            op == CompositeDestinationIn || op == CompositeDestinationAtop;
}

void CanvasRenderingContext2DSkia::ClearCanvas() {
    FloatRect canvas_rect = {{0, 0}, contentSize()};
    CheckOverdraw(canvas_rect, kNoImage, kClipFill);
    auto c = drawingContext();
    if (c)
        c->clear(Color4F::TRANSPARENT);
}

void CanvasRenderingContext2DSkia::CheckOverdraw(const FloatRect &rect, ImageType image_type, DrawType draw_type) {
    auto c = drawingContext();
    if (!c)
        return;

    SkRect device_rect;
    if (draw_type == kUntransformedUnclippedFill) {
        device_rect = rect;
    } else {
        if (currentState().hasComplexClip)
            return;

        IntRect bounds;
        if (!c->getDeviceClipBounds(&bounds))
            return;
        device_rect = SkRect::Make(bounds);
    }

    const SkImageInfo &image_info = c->getSkCanvas()->imageInfo();
    if (!device_rect.contains(SkRect::MakeWH(image_info.width(), image_info.height())))
        return;

    // With source over, we need to certify that alpha == 0xFF for all pixels
    if (image_type == kNonOpaqueImage)
        return;

    WillOverwriteCanvas();
}

void CanvasRenderingContext2DSkia::WillOverwriteCanvas() {
    // TODO: reset recording
    SkipQueuedDrawCommands();
}

void CanvasRenderingContext2DSkia::SkipQueuedDrawCommands() {
    if (have_recorded_draw_commands_) {
        m_recorder.finishRecordingAsPicture();
        StartRecording();
        have_recorded_draw_commands_ = false;
    }
}

void CanvasRenderingContext2DSkia::StartRecording() const {
    have_recorded_draw_commands_ = false;

    auto canvas = m_recorder.beginRecording(FloatRect(FloatPoint(), contentSize()));
    if (!canvas) {
        m_drawingContext.release();
        return;
    }
    // Always save an initial frame, to support resetting the top level matrix
    // and clip.
    canvas->save();
    RestoreMatrixClipStack(canvas);
    if (m_drawingContext) {
        m_drawingContext->resetSkCanvas(canvas);
    } else {
        m_drawingContext.reset(new GraphicsContextSkia(canvas));
    }
}

void CanvasRenderingContext2DSkia::FlushRecording() {
    if (have_recorded_draw_commands_ && GetOrCreateResourceProvider()) {
        auto picture = m_recorder.finishRecordingAsPicture();
        m_resourceProvider->playback(picture);
        StartRecording();
    }
}

void CanvasRenderingContext2DSkia::DisableAcceleration() {
    if (IsAccelerated()) {
        sk_sp<SkImage> snapshot = GetImage(kPreferNoAcceleration); //TODO: snapshot should be cached ?
        if (snapshot) {
            m_resourceProvider.reset();
            if (GetOrCreateResourceProvider(kPreferNoAcceleration)) {
                m_resourceProvider->drawImage(snapshot, 0, 0);
            }
        }
    }
}


void CanvasRenderingContext2DSkia::RestoreMatrixClipStack(SkCanvas *c) const {
    if (!c)
        return;

    for (auto curr_state = m_stateStack.begin(); curr_state < m_stateStack.end(); curr_state++) {
        c->setMatrix(SkMatrix::I());
        curr_state->PlaybackClips(c);
        c->setMatrix(curr_state->transform);
        c->save();
    }

    c->restore();
}

template<typename DrawFunc>
void CanvasRenderingContext2DSkia::CompositedDraw(const DrawFunc &draw_func, GraphicsContextSkia *c) {
    AffineTransform ctm = c->getTransform();
    c->setTransform(AffineTransform());
    SkPaint composite_flags;
    composite_flags.setBlendMode(WebCoreCompositeToSkiaComposite(currentState().globalComposite));
    if (shouldDrawShadows()) {
        // unroll into two independently composited passes if drawing shadows
        SkAutoCanvasRestore stateSaver(c->getSkCanvas(), false);

        c->SaveLayer(nullptr, &composite_flags);
        applyShadow(kDrawShadowOnly);
        c->setCompositeOperation(CompositeOperator::CompositeSourceOver);
        c->setTransform(ctm);
        draw_func(c);
    }

    applyShadow(kDrawForegroundOnly);
    c->SaveLayer(nullptr, &composite_flags);
    c->setCompositeOperation(CompositeOperator::CompositeSourceOver);
    c->setTransform(ctm);
    draw_func(c);
    c->restore();
    c->setTransform(ctm);
}

bool CanvasRenderingContext2DSkia::ComputeDirtyRect(
        const FloatRect &local_rect,
        const IntRect &transformed_clip_bounds,
        IntRect *dirty_rect) {
    FloatRect canvas_rect = currentState().transform.mapRect(local_rect);

    if (currentState().shadowColor.a) {
        FloatRect shadow_rect(canvas_rect);
        shadow_rect.move(currentState().shadowOffset);
        shadow_rect.inflate(currentState().shadowBlur);
        canvas_rect.unite(shadow_rect);
    }

    IntRect canvas_i_rect = roundedIntRect(canvas_rect);
    if (!canvas_i_rect.intersectsRect(transformed_clip_bounds))
        return false;

    if (dirty_rect)
        *dirty_rect = canvas_i_rect;

    return true;
}

template<typename DrawFunc, typename ContainsFunc>
bool CanvasRenderingContext2DSkia::Draw(const DrawFunc &draw_func,
                                        const ContainsFunc &draw_covers_clip_bounds,
                                        const FloatRect &bounds,
                                        const CanvasStyle &style,
                                        ImageType image_type) {
    if (!currentState().transform.isInvertible())
        return false;

    IntRect clip_bounds;
    if (!drawingContext() || !drawingContext()->getDeviceClipBounds(&clip_bounds))
        return false;

    // If gradient size is zero, then paint nothing.
    if (style.isGradientStyle()) {
        if (style.getCanvasGradient()->isZeroSize())
            return false;
    }

    if (IsFullCanvasCompositeMode(currentState().globalComposite)) {
        CompositedDraw(draw_func, drawingContext());
        DidDraw(clip_bounds);
    } else if (currentState().globalComposite == CompositeCopy) {
        ClearCanvas();  // takes care of checkOverdraw()
        applyShadow(kDrawForegroundOnly);
        draw_func(drawingContext());
        DidDraw(clip_bounds);
    } else {
        IntRect dirty_rect;
        if (ComputeDirtyRect(bounds, clip_bounds, &dirty_rect)) {
            applyShadow(kDrawShadowAndForeground);
            if (draw_covers_clip_bounds(clip_bounds))
                CheckOverdraw(bounds, image_type, kClipFill);
            draw_func(drawingContext());
            DidDraw(dirty_rect);
        }
    }

    return true;
}

void CanvasRenderingContext2DSkia::DidDraw(const IntRect &r) {
    if (r.isEmpty())
        return;

    have_recorded_draw_commands_ = true;

    FloatRect dirtyRect = r;

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

void CanvasRenderingContext2DSkia::enableConsumingDrawCache() {
    m_cacheConsuming = true;
}

void CanvasRenderingContext2DSkia::releaseConsumingImage() {
    m_cacheConsuming = false;
    if (m_consumingDrawCachedImage) {
        m_consumingDrawCachedImage.reset();
        m_dirtyRect = FloatRect::ZERO;
    }
}

void CanvasRenderingContext2DSkia::paintInContext(GraphicsContext *context, const FloatRect &contentBounds) {
    GraphicsContextSkia *graphicsContext = dynamic_cast<GraphicsContextSkia *>(context);
    SkCanvas *canvas = graphicsContext->getSkCanvas();

    FloatRect srcRect = mapRect(contentBounds,
                                {FloatPoint{}, this->m_node->getFrame().size},
                                {FloatPoint(), contentSize()});

    if (srcRect.isEmpty()) {
        return;
    }

    sk_sp<SkImage> image;
    if (m_cacheConsuming) {
        if (!m_consumingDrawCachedImage) {
            if (IsAccelerated()) {
                if (gpu_readback_invoked_in_current_frame_) {
                    gpu_readback_successive_frames_++;
                    gpu_readback_invoked_in_current_frame_ = false;
                } else {
                    gpu_readback_successive_frames_ = 0;
                }

                if (gpu_readback_successive_frames_ >= kGPUReadbackMinSuccessiveFrames) {
                    DisableAcceleration();
                }
            }

            m_consumingDrawCachedImage = GetImage(kPreferAcceleration);
        }
        image = m_consumingDrawCachedImage;
    } else {
        image = GetImage(kPreferAcceleration);
    }

    if (!image) {
        return;
    }

    SkPaint paint;
    paint.setBlendMode(SkBlendMode::kSrc);
    canvas->drawImageRect(image, srcRect, contentBounds, &paint);
}

GraphicsContextSkia *CanvasRenderingContext2DSkia::drawingContext() const {
    if (!m_drawingContext) {
        createDrawingContext();
    }

    return m_drawingContext.get();
}

void CanvasRenderingContext2DSkia::createDrawingContext() const {
    if (!contentSize().isEmpty()) {
        StartRecording();
        m_drawingContext->setImageInterpolationQuality(InterpolationLow);
        m_drawingContext->setStrokeThickness(1);
        m_resourceProvider.reset();
    }
}

bool CanvasRenderingContext2DSkia::shouldDrawShadows() const {
    auto &curState = currentState();
    return curState.shadowColor.a &&
            (curState.shadowBlur || !curState.shadowOffset.isEmpty());
}

void CanvasRenderingContext2DSkia::applyShadow(ShadowMode shadowMode) {
    if (!shouldDrawShadows()) {
        return;
    }
    auto c = drawingContext();
    auto &curState = currentState();
    c->setShadow(curState.shadowOffset, curState.shadowBlur, curState.shadowColor, shadowMode);
}


ResourceProvider *CanvasRenderingContext2DSkia::GetOrCreateResourceProvider(AccelerationHint hint) {
    if (!m_resourceProvider && !contentSize().isEmpty()) {
        if (hint == kPreferAcceleration && m_hostPageID >= 0) {
            scoped_refptr<GLResource> resource = GPUResourceManager::sharedManger()->resourceForPage(m_hostPageID);
            if (resource) {
                m_resourceProvider = ResourceProvider::CreateOffscreenResourceProvider(
                        IntSize(contentSize()),
                        true,
                        resource);
            }
        }

        if (!m_resourceProvider) {
            m_resourceProvider = ResourceProvider::CreateOffscreenResourceProvider(IntSize(contentSize()), false);
        }

    }

    return m_resourceProvider.get();
}

void CanvasRenderingContext2DSkia::reset() {
    m_stateStack.clear();
    m_stateStack.push_back(CanvasRenderingContext2DState());
    m_path->clear();
    m_resourceProvider.reset();
    m_drawingContext.reset();
}

}