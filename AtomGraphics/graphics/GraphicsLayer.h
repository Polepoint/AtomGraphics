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

#include "GraphicsLayerBackingStore.h"
#include "GraphicsPage.h"
#include "PlatformLayer.h"
#include "node/AtomNode.h"

namespace AtomGraphics {

    class Node;

    class GraphicsLayerBackingStore;

    class GraphicsPage;

    class GraphicsLayer {

    public:

        GraphicsLayer(PlatformLayer *platformLayer, Node *rootNode);

        void scheduleLayerFlush();

        void buildBackingStoreFlushContexts();

        /**
         * 将layer上的元素渲染到context上
         */
        void drawLayerContents(GraphicsContext *context);

        /**
         * 应用backingStore中存储的图像信息到当前layer上
         */
        void applyBackingStore();

        void setGraphicsPage(GraphicsPage *page);

        void setLayerContentsDirty(bool dirty);

        void addSublayer(GraphicsLayer *layer);

        void removeSublayer(GraphicsLayer *layer);

        PlatformLayer *getPlatformLayer() {
            return m_platformLayer;
        }

        Node *getRootNode() {
            return m_rootNode;
        }

    protected:
        GraphicsLayerBackingStore *m_backingStore{nullptr};

        Node *m_rootNode;
        PlatformLayer *m_platformLayer{nullptr};
    private:
        long m_layerID;
        bool m_layerContentsDirty{false};
        GraphicsPage *m_page{nullptr};
        std::vector<GraphicsLayer *> m_sublayers;

        void resetPage();

        friend class GraphicsPage;
    };
}


#endif //ATOMGRAPHICS_GRAPHICSLAYER_H
