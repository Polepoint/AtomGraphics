//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <dispatch/dispatch.h>
#import "GraphicsContentFlushController.h"
#import "DisplayLinkCA.h"

namespace AtomGraphics {

    static dispatch_queue_t flushCommitQueue;

    GraphicsContentFlushController::GraphicsContentFlushController(GraphicsPageContext *pageContext)
            : m_pageContext(pageContext), m_flushTimer(new Timer(*this, &GraphicsContentFlushController::flushLayers)) {
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            flushCommitQueue = dispatch_queue_create("neo.AtomGraphics.CommitQueue", DISPATCH_QUEUE_SERIAL);
        });
    }

    void GraphicsContentFlushController::flushLayers() {

        for (auto page : m_pages) {
            page->buildPendingFlushContexts();
        }

        std::vector<GraphicsContext *> pendingFlushContexts;
        for (auto page : m_pages) {
            if (GraphicsContext *contextPaddingFlush = page->takePendingFlushContext()) {
                pendingFlushContexts.push_back(contextPaddingFlush);
            }
        }

        //GraphicsContext其实是否flush并没有关系，如果主线程阻塞时，那子线程应该容易被优先执行
        //子线程提前flush context，可以减少主线程flush花费的时间
        GraphicsContentFlushController *thisRef = this;
        if (!pendingFlushContexts.empty()) {
            dispatch_async(flushCommitQueue, [thisRef, pendingFlushContexts] {
                for (auto pendingFlushContext :pendingFlushContexts) {
                    pendingFlushContext->flush();
                }
            });
        }

        //TODO: invoke with timer in RunLoop
        dispatch_async(dispatch_get_main_queue(), [thisRef] {
            thisRef->commitLayerContent();
        });
    }

    DisplayLink *GraphicsContentFlushController::displayLinkHandler() {
        if (!m_displayLink) {
            m_displayLink = new DisplayLinkCA(this);
        }

        return m_displayLink;
    }
}



