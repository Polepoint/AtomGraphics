//
// Created by neo on 2019/2/13.
//

#ifndef ATOMGRAPHICS_TRANSACTIONAPPLIER_H
#define ATOMGRAPHICS_TRANSACTIONAPPLIER_H

#include "graphics/Transaction.h"
#include "CompositingManager.h"

namespace AtomGraphics {

class TransactionApplier {

public:

    static void applyTransaction(const Transaction &transaction, CompositingManager *manager);

private:

    static void applyTransactionProperties(const TransactionProperties &properties,
                                           CompositingManager *manager,
                                           const Transaction &transaction);

};

}


#endif //ATOMGRAPHICS_TRANSACTIONAPPLIER_H
