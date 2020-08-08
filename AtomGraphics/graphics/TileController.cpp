//
// Created by neo on 2018/11/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "TileController.h"
#include "PlatformTileLayer.h"

namespace AtomGraphics {


TileController::TileController(PlatformTileLayer *layer)
        : m_owner(layer), m_tileGrid(new TileGrid(*this)) {

}

IntSize TileController::tileSize() {

    return IntSize(kDefaultTileSize, kDefaultTileSize);
}

std::unique_ptr<TileGridLayer> TileController::createTileLayer(const IntRect &tileRect) {
    std::unique_ptr<TileGridLayer> layer;
    if (auto layerFromPool = m_layerPool.takeLayerWithSize(IntSize(tileRect.size))) {
        layerFromPool->setPosition(FloatPoint(tileRect.origin.x, tileRect.origin.y));
        m_owner->addSublayer(layerFromPool.get());
        layer = std::move(layerFromPool);
    } else {
        layer = std::make_unique<TileGridLayer>();
    }

    m_owner->addSublayer(layer.get());
    layer->setPosition(tileRect.origin);
    layer->setBounds(FloatRect(0, 0, tileRect.size.width, tileRect.size.height));
    layer->setContentsScale(m_owner->getContentScale());
    layer->setNeedsDisplay();
    return layer;
}

void TileController::moveLayerToLayerPool(std::unique_ptr<TileGridLayer> layer) {
    m_layerPool.addLayer(std::move(layer));
}

IntRect TileController::boundsAtLastRevalidate() {
    return m_boundsAtLastRevalidate;
}

void TileController::revalidateTiles() {
    tileGrid().revalidateTiles();
}

void TileController::didRevalidateTiles() {
    m_boundsAtLastRevalidate = m_bounds;
}

void TileController::setCoverageRect(const FloatRect &rect) {
    if (m_coverageRect.equals(rect)) {
        return;
    }

    m_coverageRect = rect;
    setNeedsRevalidateTiles();
}

void TileController::setNeedsDisplay() {
    tileGrid().setNeedsDisplay();
}

void TileController::setNeedsDisplayInRect(const FloatRect &rect) {
    tileGrid().setNeedsDisplayInRect(rect);
}

void TileController::setNeedsRevalidateTiles() {
    owningGraphicsLayer()->setNeedsToRevalidateTiles();
}

float TileController::contentsScale() const {
    return tileGrid().scale() * Device::DeviceScaleFactor();
}

void TileController::setContentsScale(float contentScale) const {

    float deviceScaleFactor = Device::DeviceScaleFactor();
    contentScale /= deviceScaleFactor;

    if (tileGrid().scale() == contentScale)
        return;

    tileGrid().setScale(contentScale);
    tileGrid().setNeedsDisplay();
}

}