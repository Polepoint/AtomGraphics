//
// Created by neo on 2018/7/6.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYER_H
#define ATOMGRAPHICS_PLATFORMLAYER_H

/**
 * PlatformLayer 负责暂存异步渲染结果，各种layer的改变状态等
 * PlatformLayer 可以认为是平台layer/view的替身
 * 在iOS中指CALayer或者子类
 * 在Android中暂时指代的是View
 */
#include "math/AtomGeometry.h"
#include "platform/AtomPlatformConfig.h"
#include "LayerProperties.h"
#include "GraphicsContext.h"
#include <vector>

namespace AtomGraphics {

class GraphicsLayer;

class Transaction;

class PlatformLayer {

public:
    
    PlatformLayer();
    
    virtual ~PlatformLayer() {}
    
    virtual void commitLayerChanges();
    
    const LayerProperties &getLayerProperties() const {
        return m_properties;
    }
    
    void buildTransaction(Transaction &transaction);
    
    void collectPendingFlushContext(std::vector<std::unique_ptr<GraphicsContext>> &contextList);
    
    void setGraphicsLayer(GraphicsLayer *graphicsLayer);
    
    GraphicsLayer *getGraphicsLayer() const {
        return m_graphicsLayer;
    }
    
    long getLayerID() {
        return m_properties.layerID;
    }
    
    virtual void setVisibleRect(const FloatRect &viewRect);
    
    const FloatRect &getBounds() const {
        return m_properties.bounds;
    }
    
    virtual void setBounds(const FloatRect &bounds);
    
    float getContentScale() const {
        return m_properties.contentsScale;
    }
    
    virtual void setContentsScale(float contentScale);
    
    const FloatPoint &getPosition() const {
        return m_properties.position;
    }
    
    void setPosition(const FloatPoint &position);
    
    virtual void setNeedsDisplay();
    
    virtual void setNeedsDisplayInRect(const FloatRect &rect);
    
    void setBackingStoreAttached(bool attached);
    
    void ensureBackingStore();
    
    void updateBackingStore();
    
    void addSublayer(PlatformLayer *sublayer);
    
    void removeSublayer(PlatformLayer *sublayer);
    
    void removeFromSuperlayer();
    
    PlatformLayer *superlayer() {
        return m_superlayer;
    }
    
    virtual void updateContentScale(float scale);
    
    virtual void updateBounds();
    
    virtual void updateCoverage();
    
    virtual void updateTiles();
    
    virtual void updateDrawsContent();

protected:
    
    FloatRect m_visibleRect;
    
    GraphicsLayer *m_graphicsLayer{nullptr};
    
    LayerProperties m_properties;
    LayerChangeFlags m_uncommittedChanges{CoverageRectChanged};
    
    PlatformLayer *m_superlayer;
    std::vector<PlatformLayer *> m_sublayers;
    
    void addUncommittedChanges(LayerChangeFlags flags);
    
    friend class PlatformLayerBackingStore;
};
}

#endif //ATOMGRAPHICS_PLATFORMLAYER_H
