//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

/**
 * GraphicsPage定义为一个Graphics的最外层容器, 也是GraphicsLayer的容器
 */

#ifndef ATOMGRAPHICS_GRAPHICSPAGE_H
#define ATOMGRAPHICS_GRAPHICSPAGE_H

#include "GraphicsContentFlushController.h"
#include "base/AtomTypes.h"

namespace AtomGraphics {

class GraphicsContext;

class GraphicsLayer;

class Transaction;

struct GraphicsPageViewInfo {
    FloatRect bounds;
    FloatRect visibleRect;
    float contentsScaleFactor;
    float transformScale;

    GraphicsPageViewInfo(FloatRect bounds, FloatRect viewRect, float scale, float transformScale)
            : bounds(bounds), visibleRect(viewRect), contentsScaleFactor(scale), transformScale(transformScale) {
    }
};

class GraphicsPage {

public:

    GraphicsPage();

    GraphicsPage(GraphicsLayer *rootLayer);

    ~GraphicsPage();

    void setRootLayer(GraphicsLayer *rootLayer);

    long pageID() const;

    void setPageSize(const FloatSize &pageSize);

    void updateVisibleContentRects(const GraphicsPageViewInfo &viewInfo);

    /**
     * 准备即将更新用的内容
     */
    void buildTransaction(Transaction &transaction);

    GraphicsLayer *rootLayer() const;

    void release();

    bool visible() const {
        return m_visible;
    }

    void setVisible(bool visible) {
        m_visible = visible;
    }

private:

    long m_pageID;
    float m_lastTransformScale{1};
    bool m_visible{false};

    FloatSize m_pageSize{FloatSize::ZERO};
    GraphicsLayer *m_rootLayer{nullptr};

    friend class GraphicsContentFlushController;
};
}


#endif //ATOMGRAPHICS_GRAPHICSPAGE_H
