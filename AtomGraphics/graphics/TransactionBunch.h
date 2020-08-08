//
// Created by neo on 2018/11/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TRANSACTIONBUNCH_H
#define ATOMGRAPHICS_TRANSACTIONBUNCH_H

#include "memory/RefCounted.h"
#include "Transaction.h"
#include <map>

namespace AtomGraphics {

class TransactionBunch : public RefCountedThreadSafe<TransactionBunch> {

public:

    void addTransaction(std::unique_ptr<Transaction> transaction, long pageID);

    Transaction *getTransaction(long pageID);

private:

    std::map<long, std::unique_ptr<Transaction>> m_transactions;

};

}


#endif //ATOMGRAPHICS_TRANSACTIONBUNCH_H
