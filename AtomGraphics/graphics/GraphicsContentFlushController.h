//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTENTFLUSHCONTROLLER_H
#define ATOMGRAPHICS_GRAPHICSCONTENTFLUSHCONTROLLER_H

#include <set>
#include "DisplayRefreshMonitor.h"
#include "DisplayLink.h"
#include "GraphicsPage.h"
#include "GraphicsContext.h"
#include "thread/Timer.h"

enum DidUpdateMessageState {
    DoesNotNeedDidUpdate, NeedsDidUpdate, MissedCommit
};

namespace AtomGraphics {

    class DisplayLink;

    class GraphicsPage;

    class GraphicsPageContext;

    class DisplayRefreshMonitor;

    class GraphicsContentFlushController {

    public:

        GraphicsContentFlushController(GraphicsPageContext *pageContext);

        void didRefreshDisplay();

        void scheduleLayerFlush();

        void flushLayers();

        void commitLayerContent();

        void addPage(GraphicsPage *page);

        void removePage(GraphicsPage *page);

        DisplayRefreshMonitor *refreshMonitor() {
            return m_refreshMonitor;
        }

        void registerDisplayRefreshMonitor(DisplayRefreshMonitor *displayRefreshMonitor);

        void unregisterDisplayRefreshMonitor(DisplayRefreshMonitor *displayRefreshMonitor);

    private:

        GraphicsPageContext *m_pageContext;
        DidUpdateMessageState m_didUpdateMessageState;
        DisplayLink *m_displayLink{nullptr};
        std::set<GraphicsPage *> m_pages;
        DisplayRefreshMonitor *m_refreshMonitor{nullptr};
        Timer *m_flushTimer;
        bool m_initialized{false};

        DisplayLink *displayLinkHandler();

        void didUpdate();
    };
}

#endif //ATOMGRAPHICS_GRAPHICSCONTENTFLUSHCONTROLLER_H
