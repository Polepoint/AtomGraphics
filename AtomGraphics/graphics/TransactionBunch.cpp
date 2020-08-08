//
// Created by neo on 2018/11/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "TransactionBunch.h"


namespace AtomGraphics {

void TransactionBunch::addTransaction(std::unique_ptr<Transaction> transaction, long pageID) {
    m_transactions.insert(std::make_pair(pageID, std::move(transaction)));
}

Transaction *TransactionBunch::getTransaction(long pageID) {

    for (auto &it : m_transactions) {
        if (it.first == pageID) {
            return it.second.get();
        }
    }

    return nullptr;
}

}