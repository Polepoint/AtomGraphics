//
// Created by neo on 2018/9/1.
//

#include "CanvasNodeSkia.h"

namespace AtomGraphics {

std::shared_ptr<CanvasRenderingContext2D> CanvasNodeSkia::createContext2d() {
    return std::shared_ptr<CanvasRenderingContext2DSkia>(new CanvasRenderingContext2DSkia(this));
}


CanvasRenderingContext2DSkia *CanvasNodeSkia::context2DSkia() {
    return dynamic_cast<CanvasRenderingContext2DSkia *>(m_canvasContext2d.get());
}

void CanvasNodeSkia::setFrame(const FloatRect &frame) {
    CanvasNode::setFrame(frame);
    auto ctx = context2DSkia();
    if (ctx)
        ctx->reset();
}

void CanvasNodeSkia::willDrawContent() {
    if (m_canvasContext2d) {
        context2DSkia()->enableConsumingDrawCache();
    }
}

void CanvasNodeSkia::drawContent(GraphicsContext *context, const FloatRect &bounds) {
    if (m_canvasContext2d) {
        context2DSkia()->paintInContext(context, bounds);
    }
}

void CanvasNodeSkia::didEndDrawingContent() {
    if (m_canvasContext2d) {
        context2DSkia()->releaseConsumingImage();
    }
}

}