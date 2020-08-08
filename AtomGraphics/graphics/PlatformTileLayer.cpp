//
// Created by neo on 2018/11/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "PlatformTileLayer.h"
#include "GraphicsContentFlushController.h"

namespace AtomGraphics {

PlatformTileLayer::PlatformTileLayer() : PlatformLayer(), m_tileController(this) {

}

void PlatformTileLayer::updateContentScale(float scale) {
    m_tileController.setContentsScale(scale);
}

void PlatformTileLayer::updateBounds() {
    m_tileController.setBounds(m_properties.bounds);
}

void PlatformTileLayer::updateCoverage() {
    m_tileController.setCoverageRect(m_visibleRect);
    PlatformLayer::updateCoverage();
}

void PlatformTileLayer::updateTiles() {
    m_tileController.revalidateTiles();
}

void PlatformTileLayer::setNeedsToRevalidateTiles() {
    addUncommittedChanges(TilingAreaChanged);
}

void PlatformTileLayer::setNeedsDisplay() {
    m_tileController.setNeedsDisplay();
    GraphicsContentFlushController::SharedInstance()->scheduleLayerFlush();
}

void PlatformTileLayer::setNeedsDisplayInRect(const FloatRect &rect) {
    m_tileController.setNeedsDisplayInRect(enclosingIntRect(rect));
    GraphicsContentFlushController::SharedInstance()->scheduleLayerFlush();
}

}