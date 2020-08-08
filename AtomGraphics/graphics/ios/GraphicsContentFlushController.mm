//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsContentFlushController.h"
#import "ContentViewUpdateDispatcher.h"
#import "DisplayLinkCA.h"
#import "PlatformLayerBackingStoreFlusher.h"
#import "GraphicsLayer.h"

namespace AtomGraphics {

dispatch_queue_t GraphicsContentFlushController::s_flushCommitQueue = 0;

void GraphicsContentFlushController::commitTransaction(scoped_refptr<TransactionBunch> transactionBunch) {

    [[ContentViewUpdateDispatcher singleton] dispatchTransaction:transactionBunch.get()];

    this->didCommitTransaction();
}

void GraphicsContentFlushController::commitFlush() {
    std::vector<std::unique_ptr<GraphicsContext>> pendingFlushContexts;
    for (auto page : m_pages) {
        page->m_rootLayer->getPlatformLayer()->collectPendingFlushContext(pendingFlushContexts);
    }

    if (!pendingFlushContexts.empty()) {
        scoped_refptr<PlatformLayerBackingStoreFlusher> flusher = MakeRefCounted<PlatformLayerBackingStoreFlusher>(
                std::move(pendingFlushContexts));
        dispatch_async(s_flushCommitQueue, [flusher] {
            for (auto &pendingFlushContext :flusher->m_contexts) {
                pendingFlushContext->flush();
            }
        });
    }
}

DisplayLink *GraphicsContentFlushController::createDisplayLink() {
    return new DisplayLinkCA(this);
}

}



