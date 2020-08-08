//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTENTFLUSHCONTROLLER_H
#define ATOMGRAPHICS_GRAPHICSCONTENTFLUSHCONTROLLER_H

#include <set>
#include "DisplayLink.h"
#include "DisplayRefreshMonitor.h"
#include "GraphicsContext.h"
#include "TransactionBunch.h"
#include "thread/Timer.h"

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#import <dispatch/queue.h>

#endif

enum DidUpdateMessageState {
    DoesNotNeedDidUpdate, NeedsDidUpdate, MissedCommit
};

namespace AtomGraphics {

class GraphicsPage;

class GraphicsContentFlushController {

public:

    static GraphicsContentFlushController *SharedInstance();

    GraphicsContentFlushController();

    void didRefreshDisplay();

    void scheduleLayerFlush();

    void flushLayers();

#if PLATFORM(IOS)

    void commitFlush();

#endif

    void commitTransaction(scoped_refptr<TransactionBunch> transactionBunch);

    void didCommitTransaction();

    void addPage(GraphicsPage *page);

    void removePage(GraphicsPage *page);

    DisplayRefreshMonitor *refreshMonitor();

private:

#if PLATFORM(IOS)
    static dispatch_queue_t s_flushCommitQueue;
#endif

    DidUpdateMessageState m_didUpdateMessageState{DoesNotNeedDidUpdate};
    std::unique_ptr<DisplayLink> m_displayLink;
    std::unique_ptr<DisplayRefreshMonitor> m_refreshMonitor;
    std::set<GraphicsPage *> m_pages;
    Timer m_flushTimer;
    bool m_initialized{false};
    bool m_hadFlushDeferredWhileWaitingForBackingStoreSwap{false};

    DisplayLink *displayLinkHandler();

    DisplayLink *createDisplayLink();

    void didUpdate();
};

}

#endif //ATOMGRAPHICS_GRAPHICSCONTENTFLUSHCONTROLLER_H
