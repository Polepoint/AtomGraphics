//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TRANSACTIONAPPLIER_H
#define ATOMGRAPHICS_TRANSACTIONAPPLIER_H

#import "TransactionProperties.h"
#import "Transaction.h"

@class CompositingView;
@class CompositingManager;

namespace AtomGraphics {

class TransactionApplier {

public:
    
    static void applyTransaction(Transaction *transaction, CompositingManager *manager);
    
private:
    
    static void applyTransactionProperties(const TransactionProperties &properties, CompositingManager *manager, Transaction *transaction);
    
};

}


#endif //ATOMGRAPHICS_TRANSACTIONAPPLIER_H
