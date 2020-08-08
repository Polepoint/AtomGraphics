//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CanvasNode.h"
#include "CanvasRenderingContext2D.h"
#include "platform/AtomPlatformConfig.h"
#include "base/Device.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CGBitmapContext.h>

#endif

namespace AtomGraphics {

CanvasNode::CanvasNode() : m_deviceScaleFactor(Device::DeviceScaleFactor()) {

}

std::shared_ptr<CanvasRenderingContext2D> CanvasNode::getContext2d() {
    if (!m_canvasContext2d) {
        m_canvasContext2d = createContext2d();
    }
    
    return m_canvasContext2d;
}

void CanvasNode::setFrame(const FloatRect &frame) {
    Node::setFrame(frame);
    if (m_contentSize.isEmpty()) {
        m_contentSize = frame.size;
        m_contentSize.scale(m_deviceScaleFactor);
    }
}

const FloatSize &CanvasNode::contentSize() const {
    return m_contentSize;
}

float CanvasNode::contentWidth() const {
    return m_contentSize.width;
}

void CanvasNode::setContentWidth(float width) {
    if (m_contentSize.width == width) {
        return;
    }
    
    m_contentSize.width = width;
    if (m_canvasContext2d) {
        m_canvasContext2d->reset();
    }
};

float CanvasNode::contentHeight() const {
    return m_contentSize.height;
}

void CanvasNode::setContentHeight(float height) {
    if (m_contentSize.height == height) {
        return;
    }
    
    m_contentSize.height = height;
    if (m_canvasContext2d) {
        m_canvasContext2d->reset();
    }
}

}

