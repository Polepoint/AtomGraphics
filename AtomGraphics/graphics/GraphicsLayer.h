//
// Created by neo on 2018/7/5.
// Copyright (c) 2018 neo. All rights reserved.
//

/**
 * GraphicsLayer定义为Node的容器
 * Node所定义的所有图形最后也应该直接渲染在GraphicsLayer上
 * layer的设计初衷是希望做成可以像View一样多层嵌套，添加sublayer
 * 但是backingStore的更新逻辑没有设计好
 */

#ifndef ATOMGRAPHICS_GRAPHICSLAYER_H
#define ATOMGRAPHICS_GRAPHICSLAYER_H

#include "GraphicsPage.h"
#include "PlatformLayer.h"
#include "node/Node.h"

namespace AtomGraphics {

class Node;

class PlatformLayerBackingStore;

class GraphicsPage;

class Transaction;

class GraphicsLayer {

public:

    GraphicsLayer(std::unique_ptr<PlatformLayer> platformLayer);

    ~GraphicsLayer();

    void setBounds(const FloatRect &bounds);

    void buildTransaction(Transaction &transaction);

    void commitLayerChanges();

    long getLayerID() const {
        return m_layerID;
    }

    void setGraphicsPage(GraphicsPage *page);

    void addSublayer(GraphicsLayer *layer);

    void removeSublayer(GraphicsLayer *layer);

    PlatformLayer *getPlatformLayer() {
        return m_platformLayer.get();
    }

    std::shared_ptr<Node> getRootNode() {
        return m_rootNode;
    }

    void setRootNode(std::shared_ptr<Node> node);

    float getTransformScale() const {
        return m_transformScale;
    }

    FloatRect platformLayerFrameInGraphics(PlatformLayer *);

    void updateBounds(const FloatRect &bounds);

    void updateVisibleRect(const FloatRect &viewRect);

    void updateContentsScale(float contentsScale);

    void updateTransformScale(float transformScale);

protected:
    FloatRect m_bounds{FloatRect::ZERO};

    std::shared_ptr<Node> m_rootNode{nullptr};
    std::unique_ptr<PlatformLayer> m_platformLayer{nullptr};

private:
    long m_layerID;
    float m_contentsScale{1};
    float m_transformScale{1};
    FloatRect m_viewRect;
    GraphicsPage *m_page{nullptr};
    std::vector<GraphicsLayer *> m_sublayers;

    void resetPage();

    friend class GraphicsPage;
};
}


#endif //ATOMGRAPHICS_GRAPHICSLAYER_H
