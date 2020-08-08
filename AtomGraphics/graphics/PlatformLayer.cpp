//
// Created by neo on 20181/11/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "PlatformLayer.h"
#include "Transaction.h"
#include "PlatformLayerBackingStore.h"
#include "GraphicsContentFlushController.h"
#include "GraphicsLayer.h"

namespace AtomGraphics {

PlatformLayer::PlatformLayer() {
    static long layerIDCount = layerIDNone + 1;
    m_properties.layerID = layerIDCount++;
}

void PlatformLayer::addUncommittedChanges(LayerChangeFlags flags) {
    m_uncommittedChanges |= flags;
}

void PlatformLayer::commitLayerChanges() {
    if (!m_uncommittedChanges) {
        return;
    }
    
    if (m_uncommittedChanges & ContentsScaleChanged) {
        updateContentScale(m_properties.contentsScale);
    }
    
    if (m_uncommittedChanges & BoundsChanged) {
        updateBounds();
    }
    
    if (m_uncommittedChanges & DrawsContentChanged) {
        updateDrawsContent();
    }
    
    if (m_uncommittedChanges & CoverageRectChanged) {
        updateCoverage();
    }
    
    if (m_uncommittedChanges & TilingAreaChanged) {
        updateTiles();
    }
    
    m_uncommittedChanges = NoChange;
    
    for (auto sublayer: m_sublayers) {
        sublayer->commitLayerChanges();
    }
}

void PlatformLayer::updateContentScale(float scale) {

}

void PlatformLayer::updateBounds() {

}

void PlatformLayer::updateCoverage() {
    if (m_visibleRect.intersectsRect(m_properties.bounds)) {
        //requires attaching backingStore
        this->setBackingStoreAttached(true);
    }
}

void PlatformLayer::updateTiles() {

}

void PlatformLayer::updateDrawsContent() {

}

void PlatformLayer::buildTransaction(Transaction &transaction) {
    if (m_properties.backingStore && m_properties.backingStore->display()) {
        m_properties.notePropertiesChanged(BackingStoreChanged);
    }
    
    transaction.addLayerProperties(m_properties);
    m_properties.reset();
    
    for (auto sublayer : m_sublayers) {
        sublayer->buildTransaction(transaction);
    }
}

void PlatformLayer::collectPendingFlushContext(std::vector<std::unique_ptr<GraphicsContext>> &contextList) {
    if (m_properties.backingStore) {
        auto ctx = std::move(m_properties.backingStore->takeFrontContextPendingFlush());
        if (ctx) {
            contextList.push_back(std::move(ctx));
        }
    }

    if (m_sublayers.size()) {
        for (auto layer : m_sublayers) {
            layer->collectPendingFlushContext(contextList);
        }
    }
}

void PlatformLayer::setGraphicsLayer(GraphicsLayer *graphicsLayer) {
    m_graphicsLayer = graphicsLayer;
    for (auto child : m_sublayers) {
        child->setGraphicsLayer(graphicsLayer);
    }
}

void PlatformLayer::setVisibleRect(const FloatRect &viewRect) {
    if (m_visibleRect.equals(viewRect)) {
        return;
    }
    m_visibleRect = viewRect;
    addUncommittedChanges(CoverageRectChanged);
}

void PlatformLayer::setBounds(const FloatRect &bounds) {
    if (m_properties.bounds.equals(bounds)) {
        return;
    }
    m_properties.bounds = bounds;
    m_properties.notePropertiesChanged(BoundsChanged);
    addUncommittedChanges(BoundsChanged);
}

void PlatformLayer::setContentsScale(float contentScale) {
    if (m_properties.contentsScale == contentScale) {
        return;
    }
    m_properties.contentsScale = contentScale;
    m_properties.notePropertiesChanged(ContentsScaleChanged);
    addUncommittedChanges(ContentsScaleChanged);
}

void PlatformLayer::setPosition(const FloatPoint &position) {
    if (m_properties.position.equals(position)) {
        return;
    }
    m_properties.position = position;
    m_properties.notePropertiesChanged(PositionChanged);
}

void PlatformLayer::setNeedsDisplay() {
    setNeedsDisplayInRect(FloatRect({0, 0}, m_properties.bounds.size));
}

void PlatformLayer::setNeedsDisplayInRect(const FloatRect &rect) {
    FloatRect drawRect = FloatRect(rect);
    drawRect.intersect(FloatRect({0, 0}, m_properties.bounds.size));
    m_properties.notePropertiesChanged(BackingStoreChanged);
    
    ensureBackingStore();
    m_properties.backingStore->setNeedsDisplayInRect(drawRect);
    GraphicsContentFlushController::SharedInstance()->scheduleLayerFlush();
}

void PlatformLayer::setBackingStoreAttached(bool attached) {
    if (m_properties.backingStoreAttached == attached)
        return;
    
    m_properties.backingStoreAttached = attached;
    
    if (attached)
        setNeedsDisplay();
    else
        m_properties.backingStore = nullptr;
}

void PlatformLayer::ensureBackingStore() {
    if (!m_properties.backingStore)
        m_properties.backingStore = std::make_unique<PlatformLayerBackingStore>(this);
    
    updateBackingStore();
}

void PlatformLayer::updateBackingStore() {
    if (!m_properties.backingStore)
        return;
    
    m_properties.backingStore->ensureBackingStore(m_properties.bounds.size, m_properties.contentsScale * m_graphicsLayer->getTransformScale());
}

void PlatformLayer::addSublayer(PlatformLayer *sublayer) {
    auto iterator = std::find(m_sublayers.begin(), m_sublayers.end(), sublayer);
    if (iterator == m_sublayers.end()) {
        m_sublayers.push_back(sublayer);
        sublayer->m_superlayer = this;
        sublayer->setGraphicsLayer(m_graphicsLayer);
        sublayer->m_properties.superlayerID = m_properties.layerID;
        m_properties.notePropertiesChanged(ChildrenChanged);
        addUncommittedChanges(ChildrenChanged);
    }
}

void PlatformLayer::removeSublayer(PlatformLayer *sublayer) {
    auto iterator = std::find(m_sublayers.begin(), m_sublayers.end(), sublayer);
    if (iterator != m_sublayers.end()) {
        m_sublayers.erase(iterator);
        sublayer->m_superlayer = nullptr;
        sublayer->setGraphicsLayer(nullptr);
        sublayer->m_properties.superlayerID = layerIDNone;
        m_properties.notePropertiesChanged(ChildrenChanged);
        addUncommittedChanges(ChildrenChanged);
    }
}

void PlatformLayer::removeFromSuperlayer() {
    if (m_superlayer) {
        m_superlayer->removeSublayer(this);
    }
}

}