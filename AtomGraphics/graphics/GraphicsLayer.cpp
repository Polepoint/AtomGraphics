//
// Created by neo on 2018/7/5.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <algorithm>
#include <utility>
#include "GraphicsLayer.h"
#include "PlatformLayerBackingStore.h"
#include "PlatformLayerBackingStorePainter.h"


namespace AtomGraphics {

static long layerIDCounter = 0;

GraphicsLayer::GraphicsLayer(std::unique_ptr<PlatformLayer> platformLayer)
        : m_platformLayer(std::move(platformLayer)),
          m_layerID(layerIDCounter++) {
    m_platformLayer->setGraphicsLayer(this);
}

GraphicsLayer::~GraphicsLayer() {
    if (m_rootNode) {
        m_rootNode->setGraphicsLayer(nullptr);
    }
}


void GraphicsLayer::setBounds(const FloatRect &bounds) {
    m_bounds = bounds;
    m_platformLayer->setBounds(m_bounds);
}

void GraphicsLayer::buildTransaction(Transaction &transaction) {
    PlatformLayerBackingStorePainter::buildBackingStoreTransactionBegin();
    m_platformLayer->buildTransaction(transaction);
    PlatformLayerBackingStorePainter::buildBackingStoreTransactionEnd();
}

void GraphicsLayer::commitLayerChanges() {
    m_platformLayer->commitLayerChanges();

    for (auto sublayer: m_sublayers) {
        sublayer->commitLayerChanges();
    }
}

void GraphicsLayer::setGraphicsPage(GraphicsPage *page) {
    m_page = page;
}

void GraphicsLayer::addSublayer(GraphicsLayer *layer) {
    if (layer != this) {
        m_sublayers.push_back(layer);
        layer->m_page = m_page;
    }
}

void GraphicsLayer::removeSublayer(GraphicsLayer *layer) {
    auto it = std::find(m_sublayers.begin(), m_sublayers.end(), layer);
    if (it != m_sublayers.end()) {
        m_sublayers.erase(it);
        layer->resetPage();
    }
}

void GraphicsLayer::resetPage() {
    m_page = nullptr;
    for (auto layer : m_sublayers) {
        layer->resetPage();
    }
}

void GraphicsLayer::setRootNode(std::shared_ptr<Node> node) {
    m_rootNode = std::move(node);
    m_rootNode->setGraphicsLayer(this);
    m_rootNode->setFrame(m_bounds);
}

FloatRect GraphicsLayer::platformLayerFrameInGraphics(PlatformLayer *platformLayer) {
    if (!platformLayer) {
        return FloatRect();
    }

    PlatformLayer *layer = platformLayer;
    const FloatRect &bounds = platformLayer->getBounds();
    FloatPoint position = bounds.origin;

    while (layer && layer != m_platformLayer.get()) {
        position.add(layer->getPosition());
        layer = layer->superlayer();
    }

    return FloatRect(position, bounds.size);
}

void GraphicsLayer::updateBounds(const FloatRect &bounds) {
    if (m_bounds.equals(bounds)) {
        return;
    }
    m_bounds = bounds;
    m_platformLayer->setBounds(bounds);
    if (m_rootNode) {
        m_rootNode->setFrame(bounds);
    }
}

void GraphicsLayer::updateVisibleRect(const FloatRect &viewRect) {
    if (m_viewRect.equals(viewRect)) {
        return;
    }
    m_viewRect = viewRect;
    m_platformLayer->setVisibleRect(viewRect);
}

void GraphicsLayer::updateContentsScale(float contentsScale) {
    if (m_contentsScale == contentsScale) {
        return;
    }
    m_contentsScale = contentsScale;
    m_platformLayer->setContentsScale(contentsScale);
}

void GraphicsLayer::updateTransformScale(float transformScale) {
    if (m_transformScale == transformScale) {
        return;
    }
    m_transformScale = transformScale;
}

}