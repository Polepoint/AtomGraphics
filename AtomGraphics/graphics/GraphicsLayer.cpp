//
// Created by neo on 2018/7/5.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <algorithm>
#include "GraphicsLayer.h"


namespace AtomGraphics {

    static long layerIDCounter = 0;

    GraphicsLayer::GraphicsLayer(PlatformLayer *platformLayer, Node *rootNode)
            : m_layerID(layerIDCounter++),
              m_platformLayer(platformLayer),
              m_rootNode(rootNode) {
        m_rootNode->setGraphicsLayer(this);
    }

    void GraphicsLayer::scheduleLayerFlush() {
        if (m_page) {
            m_page->schedulePageFlush();
        }
    }

    void GraphicsLayer::buildBackingStoreFlushContexts() {
        m_backingStore->display();
    }

    void GraphicsLayer::drawLayerContents(GraphicsContext *context) {
        //TODO draw Node elements
        m_rootNode->draw(context);
        for (auto layer :m_sublayers) {
            layer->drawLayerContents(context);
        }
    }

    void GraphicsLayer::applyBackingStore() {
        m_backingStore->applyBackingStoreToLayer(m_platformLayer);
        m_layerContentsDirty = false;
    }

    void GraphicsLayer::setGraphicsPage(GraphicsPage *page) {
        m_page = page;
    }

    void GraphicsLayer::setLayerContentsDirty(bool dirty) {
        m_layerContentsDirty = dirty;
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
        for (auto layer:m_sublayers) {
            layer->resetPage();
        }
    }
}