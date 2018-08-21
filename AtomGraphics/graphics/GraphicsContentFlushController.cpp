//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsContentFlushController.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

#include "graphics/android/DisplayLinkAndroid.h"
#include <pthread.h>

#endif

namespace AtomGraphics {

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

    GraphicsContentFlushController::GraphicsContentFlushController(GraphicsPageContext *pageContext)
            : m_pageContext(pageContext), m_flushTimer(new Timer(*this, &GraphicsContentFlushController::flushLayers)) {
    }

#endif

    void GraphicsContentFlushController::didRefreshDisplay() {
        if (m_didUpdateMessageState != NeedsDidUpdate) {
            m_didUpdateMessageState = MissedCommit;
            displayLinkHandler()->pause();
            return;
        }
        m_didUpdateMessageState = DoesNotNeedDidUpdate;

        //TODO: 切换到 Graphics queue
        {
            this->didUpdate();
        }
    }

    void GraphicsContentFlushController::didUpdate() {
        //requestAnimationFrame
        for (auto page : m_pages) {
            if (page->m_updating) {
                page->didUpdate();
                page->m_updating = false;
            }
        }

        m_refreshMonitor->didUpdateLayers();
    }

    /**
     * schedule after setNeedsDisplay
     */
    void GraphicsContentFlushController::scheduleLayerFlush() {
        if (m_flushTimer->isActive()) {
            return;
        }

        //initialized?
        ThreadTimerInterval interval = m_initialized ? 0 : 0.1;
        m_initialized = true;
        m_flushTimer->startOneShot(interval);
    }

    void GraphicsContentFlushController::commitLayerContent() {
        for (auto page: m_pages) {
            if (page->m_pendingToFlush) {
                page->updateLayerContents();
                page->m_pendingToFlush = false;
            }
        }
        if (std::exchange(m_didUpdateMessageState, NeedsDidUpdate) == MissedCommit)
            didRefreshDisplay();
        displayLinkHandler()->schedule();
    }


#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

    void GraphicsContentFlushController::flushLayers() {
        for (auto page : m_pages) {
            page->buildPendingFlushContexts();
        }

        std::vector<GraphicsContext *> *pendingFlushContexts = new std::vector<GraphicsContext *>();
        for (auto page : m_pages) {
            if (GraphicsContext *contextPaddingFlush = page->takePendingFlushContext()) {
                pendingFlushContexts->push_back(contextPaddingFlush);
            }
        }

        if (pendingFlushContexts->empty()) {
            delete pendingFlushContexts;
        } else {
            pthread_t flushCommitThread = 0;
            auto callback = [](void *data) -> void * {
                std::vector<GraphicsContext *> *contents = static_cast<std::vector<GraphicsContext *> *>(data);
                for (auto pendingFlushContext :*contents) {
                    pendingFlushContext->flush();
                }
                delete contents;
                return nullptr;
            };
            pthread_create(&flushCommitThread, 0, std::move(callback), pendingFlushContexts);
        }

        GraphicsThread::uiTaskRunner()->postTask(std::bind(&GraphicsContentFlushController::commitLayerContent, this));
    }

    DisplayLink *GraphicsContentFlushController::displayLinkHandler() {
        if (!m_displayLink) {
            m_displayLink = new DisplayLinkAndroid(this, m_pageContext);
        }
        return m_displayLink;
    }

#endif

    void GraphicsContentFlushController::addPage(GraphicsPage *page) {
        m_pages.insert(page);
    }

    void GraphicsContentFlushController::removePage(GraphicsPage *page) {
        m_pages.erase(page);
    }

    void GraphicsContentFlushController::registerDisplayRefreshMonitor(
            DisplayRefreshMonitor *displayRefreshMonitor) {
        m_refreshMonitor = displayRefreshMonitor;
    }

    void GraphicsContentFlushController::unregisterDisplayRefreshMonitor(
            DisplayRefreshMonitor *displayRefreshMonitor) {
        m_refreshMonitor = nullptr;
    }
}