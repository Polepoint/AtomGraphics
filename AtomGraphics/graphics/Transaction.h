//
// Created by neo on 2018/11/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_LAYERTRANSACTION_H
#define ATOMGRAPHICS_LAYERTRANSACTION_H

#include "LayerProperties.h"
#include "TransactionProperties.h"
#include <vector>

namespace AtomGraphics {

class Transaction {

public:

    Transaction() {}

    void addLayerProperties(LayerProperties &properties) {
        m_propertyList.push_back(TransactionProperties(properties));
    }

    const std::vector<TransactionProperties> &getPropertyList() const {
        return m_propertyList;
    }

private:

    std::vector<TransactionProperties> m_propertyList;

};

}


#endif //ATOMGRAPHICS_LAYERTRANSACTION_H
