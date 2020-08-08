//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "thread/GraphicsThread.h"
#include "thread/UIThread.h"
#include "GraphicsContentFlushController.h"
#include "DisplayRefreshMonitor.h"
#include "PlatformLayer.h"
#include "PlatformLayerBackingStoreFlusher.h"
#include "GraphicsLayer.h"
#include "GraphicsPage.h"
#include "Transaction.h"
#include "TransactionBunch.h"

#if PLATFORM(IOS)

#import <dispatch/dispatch.h>

#elif PLATFORM(ANDROID)

#include <pthread.h>

#endif

namespace AtomGraphics {

GraphicsContentFlushController::GraphicsContentFlushController()
        : m_flushTimer(GraphicsThread::GraphicsThreadTaskRunner(),
                       *this,
                       &GraphicsContentFlushController::flushLayers) {
#if PLATFORM(IOS)
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        s_flushCommitQueue = dispatch_queue_create("neo.AtomGraphics.CommitQueue", DISPATCH_QUEUE_SERIAL);
    });
#endif
}

GraphicsContentFlushController *GraphicsContentFlushController::SharedInstance() {
    static GraphicsContentFlushController *sharedInstance;
    if (!sharedInstance) {
        sharedInstance = new GraphicsContentFlushController();
    }
    return sharedInstance;
}

void GraphicsContentFlushController::flushLayers() {

    scoped_refptr<TransactionBunch> bunch = MakeRefCounted<TransactionBunch>();
    for (auto page : m_pages) {

        page->rootLayer()->commitLayerChanges();

        Transaction *transaction = new Transaction();
        page->buildTransaction(*transaction);
        std::unique_ptr<Transaction> ta(transaction);
        bunch->addTransaction(std::move(ta), page->m_pageID);
    }

    GraphicsContentFlushController *thisRef = this;

#if PLATFORM(IOS)

    commitFlush();

#endif

    UIThread::UIThreadTaskRunner()->postTask([thisRef, bunch] {
        thisRef->commitTransaction(bunch);
    });
}

void GraphicsContentFlushController::didRefreshDisplay() {
    if (m_didUpdateMessageState != NeedsDidUpdate) {
        m_didUpdateMessageState = MissedCommit;
        DisplayLink *displayLink = displayLinkHandler();
        displayLink->pause();
        return;
    }
    m_didUpdateMessageState = DoesNotNeedDidUpdate;

    this->didUpdate();
}

void GraphicsContentFlushController::didUpdate() {
    //schedule update if need flush
    if (m_hadFlushDeferredWhileWaitingForBackingStoreSwap) {
        scheduleLayerFlush();
        m_hadFlushDeferredWhileWaitingForBackingStoreSwap = false;
    }

    //requestAnimationFrame
    if (m_refreshMonitor) {
        m_refreshMonitor->didUpdateLayers();
    }
}

/**
 * schedule after setNeedsDisplay
 */
void GraphicsContentFlushController::scheduleLayerFlush() {
    if (m_flushTimer.isActive()) {
        return;
    }

    //initialized?
    TimeInterval interval = m_initialized ? 0 : 0.1;
    m_initialized = true;
    m_flushTimer.startOneShot(interval);
}

void GraphicsContentFlushController::didCommitTransaction() {
    if (std::exchange(m_didUpdateMessageState, NeedsDidUpdate) == MissedCommit)
        didRefreshDisplay();
    displayLinkHandler()->schedule();
}

void GraphicsContentFlushController::addPage(GraphicsPage *page) {
    m_pages.insert(page);
}

void GraphicsContentFlushController::removePage(GraphicsPage *page) {
    m_pages.erase(page);
}

DisplayRefreshMonitor *GraphicsContentFlushController::refreshMonitor() {
    if (!m_refreshMonitor) {
        m_refreshMonitor.reset(new DisplayRefreshMonitor());
    }

    return m_refreshMonitor.get();
}

DisplayLink *GraphicsContentFlushController::displayLinkHandler() {
    if (!m_displayLink) {
        m_displayLink.reset(createDisplayLink());
    }

    return m_displayLink.get();
}


}