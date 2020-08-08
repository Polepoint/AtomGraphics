//
// Created by neo on 2018/11/20.
//


#include "android/ContentViewUpdateDispatcher.h"
#include "graphics/GraphicsContentFlushController.h"
#include "DisplayLinkAndroid.h"

namespace AtomGraphics {

void GraphicsContentFlushController::commitTransaction(scoped_refptr<TransactionBunch> transactionBunch) {

    ContentViewUpdateDispatcher::SharedInstance()->dispatchTransaction(transactionBunch);
    this->didCommitTransaction();
}

DisplayLink *GraphicsContentFlushController::createDisplayLink() {
    return new DisplayLinkAndroid(this);
}

}


