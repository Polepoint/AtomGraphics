//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsPage.h"
#include "GraphicsContentFlushController.h"
#include "GraphicsPageManager.h"

namespace AtomGraphics {

    static long pageIDCounter = 0;

    GraphicsPage::GraphicsPage(GraphicsPageContext *context, GraphicsLayer *rootLayer)
            : m_pageContext(context), m_rootLayer(rootLayer), m_pageID(pageIDCounter++) {
        m_rootLayer->setGraphicsPage(this);
        m_pageContext->flushController()->addPage(this);
        GraphicsPageManager::registerPage(this);
    }

    GraphicsPage::~GraphicsPage() {
        m_pageContext->flushController()->removePage(this);
        GraphicsPageManager::unregisterPage(this);
    }

    long GraphicsPage::pageID() const {
        return m_pageID;
    }

    void GraphicsPage::schedulePageFlush() {
        if (!m_needsFlush) {
            m_needsFlush = true;
            m_updating = true;
            m_pageContext->flushController()->scheduleLayerFlush();
        }
    }

    void GraphicsPage::didUpdate() {

    }

    void GraphicsPage::buildPendingFlushContexts() {
        /**
         * 判断layer是否发生了变化，如果变化，标记m_pendingToFlush
         */
        if (m_rootLayer->m_layerContentsDirty) {
            m_rootLayer->buildBackingStoreFlushContexts();
            m_pendingToFlush = true;
        }
    }

    GraphicsContext *GraphicsPage::takePendingFlushContext() {
        if (m_rootLayer->m_layerContentsDirty) {
            return m_rootLayer->m_backingStore->takePendingFlushContext();
        }

        return nullptr;
    }

    void GraphicsPage::updateLayerContents() {
        m_rootLayer->applyBackingStore();
        m_needsFlush = false;
    }

    GraphicsLayer *GraphicsPage::rootLayer() const {
        return m_rootLayer;
    }

    void GraphicsPage::release() {

    }
}
