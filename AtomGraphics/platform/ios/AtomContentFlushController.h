//
// Created by neo on 2018/4/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomMessageReceiver.h"
#import "AtomConnection.h"
#import "AtomLayerBackingStore.h"
//#import "AtomTimer.h"

@class AtomViewContentDisplayLinkHandler;

enum DidUpdateMessageState {
    DoesNotNeedDidUpdate, NeedsDidUpdate, MissedCommit
};

namespace AtomGraphics {

    class AtomContentFlushController : public MessageReceiver {

    public:

        AtomContentFlushController();

        void didReceiveMessage() override;

        void didRefreshDisplay();

        void scheduleLayerFlush();

        void flushLayer();

        void commitLayerContent();

        void setBackingStoreToFlush(AtomLayerBackingStore *backingStoreToFlush);

        void setContentLayer(CALayer *contentLayer);

    private:

        DidUpdateMessageState _didUpdateMessageState;
        AtomViewContentDisplayLinkHandler *_displayLinkHandler;
        dispatch_queue_t _commitQueue;
        AtomLayerBackingStore *_backingStoreToFlush;
        CALayer *_contentLayer;
//        Timer *_flushTimer;

        AtomViewContentDisplayLinkHandler *displayLinkHandler();
    };
}

