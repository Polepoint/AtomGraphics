//
// Created by neo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "CanvasNodeCG.h"
#include "CanvasNodeCG.h"


namespace AtomGraphics {

std::shared_ptr<CanvasRenderingContext2D> CanvasNodeCG::createContext2d() {
    return std::shared_ptr<CanvasRenderingContext2D>(new CanvasRenderingContext2DCG(this));
}

CanvasRenderingContext2DCG *CanvasNodeCG::context2DCG() {
    return dynamic_cast<CanvasRenderingContext2DCG *>(m_canvasContext2d.get());
}

void CanvasNodeCG::setFrame(const FloatRect &frame) {
    CanvasNode::setFrame(frame);
    if (m_canvasContext2d)
        m_canvasContext2d->reset();
}

void CanvasNodeCG::willDrawContent() {
    if (m_canvasContext2d)
        m_canvasContext2d->enableConsumingDrawCache();
}

void CanvasNodeCG::drawContent(GraphicsContext *context, const FloatRect &bounds) {
    if (m_canvasContext2d)
        m_canvasContext2d->paintInContext(context, bounds);
}

void CanvasNodeCG::didEndDrawingContent() {
    if (m_canvasContext2d)
        m_canvasContext2d->releaseConsumingImage();
}

}