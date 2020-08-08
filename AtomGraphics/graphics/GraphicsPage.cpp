//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsPage.h"
#include "GraphicsLayer.h"
#include "GraphicsContentFlushController.h"
#include "GraphicsPageManager.h"
#include "thread/GraphicsThread.h"
#include "Transaction.h"
#include "PlatformLayerBackingStore.h"

namespace AtomGraphics {

static long pageIDCounter = 0;

GraphicsPage::GraphicsPage()
        : m_pageID(pageIDCounter++) {
    GraphicsContentFlushController::SharedInstance()->addPage(this);
    GraphicsPageManager::registerPage(this);
}

GraphicsPage::GraphicsPage(GraphicsLayer *rootLayer)
        : m_pageID(pageIDCounter++), m_rootLayer(rootLayer) {
    m_rootLayer->setGraphicsPage(this);
    GraphicsContentFlushController::SharedInstance()->addPage(this);
    GraphicsPageManager::registerPage(this);
}

GraphicsPage::~GraphicsPage() {
    GraphicsContentFlushController::SharedInstance()->removePage(this);
    GraphicsPageManager::unregisterPage(this);
}

void GraphicsPage::setRootLayer(GraphicsLayer *rootLayer) {
    m_rootLayer = rootLayer;
    m_rootLayer->setGraphicsPage(this);
}

long GraphicsPage::pageID() const {
    return m_pageID;
}

void GraphicsPage::setPageSize(const FloatSize &pageSize) {
    if (!m_pageSize.equals(pageSize)) {
        m_pageSize = pageSize;
        m_rootLayer->setBounds(FloatRect(0, 0, m_pageSize.width, m_pageSize.height));
    }
}

inline int leftMostBitPosition(int number) {
    int position = -1;
    while (number > 0) {
        position++;
        number = number >> 1;
    }

    return position;
}

void GraphicsPage::updateVisibleContentRects(const GraphicsPageViewInfo &viewInfo) {
    m_rootLayer->updateBounds(viewInfo.bounds);
    m_rootLayer->updateVisibleRect(viewInfo.visibleRect);
    m_rootLayer->updateContentsScale(viewInfo.contentsScaleFactor);
    float transformScale = viewInfo.transformScale;
    if (transformScale != m_lastTransformScale) {
        if (transformScale > 1) {
            int levelsOfDetail = leftMostBitPosition(static_cast<int>(transformScale));
            int lastLevelsOfDetail = leftMostBitPosition(static_cast<int>(m_lastTransformScale));
            if (levelsOfDetail != lastLevelsOfDetail) {
                m_rootLayer->updateTransformScale(1 << levelsOfDetail);
            }
        } else if (transformScale < 1) {
            int levelsOfDetailBias = leftMostBitPosition(static_cast<int>(1 / transformScale));
            int lastLevelsOfDetailBias = leftMostBitPosition(static_cast<int>(1 / m_lastTransformScale));
            if (levelsOfDetailBias != lastLevelsOfDetailBias) {
                m_rootLayer->updateTransformScale(1.0f / (1 << levelsOfDetailBias));
            }
        } else {
            m_rootLayer->updateTransformScale(1);
        }
    }

    m_lastTransformScale = transformScale;
}

void GraphicsPage::buildTransaction(Transaction &transaction) {
    m_rootLayer->buildTransaction(transaction);
}

GraphicsLayer *GraphicsPage::rootLayer() const {
    return m_rootLayer;
}

void GraphicsPage::release() {

}

}
