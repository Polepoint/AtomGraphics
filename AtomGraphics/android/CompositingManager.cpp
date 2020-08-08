//
// Created by neo on 2019/2/13.
//

#include "CompositingManager.h"

namespace AtomGraphics {

std::shared_ptr<CompositingView> CompositingManager::createViewWithID(long viewID) {
    std::shared_ptr<CompositingView> view = std::shared_ptr<CompositingView>(new CompositingView(viewID));
    m_compositingViews.push_back(view);
    m_idSummary += viewID;
    return view;
}

std::shared_ptr<CompositingView> CompositingManager::viewWithID(long viewID) {
    for (auto it = m_compositingViews.begin(); it != m_compositingViews.end(); it++) {
        if ((*it)->viewID() == viewID) return *it;
    }

    return nullptr;
}


std::vector<long> CompositingManager::subviewIDs(long superviewID, const Transaction &transaction) {
    std::vector<long> ids;
    std::vector<TransactionProperties> propertyList = transaction.getPropertyList();
    for (TransactionProperties &properties : propertyList) {
        if (properties.m_superlayerID == superviewID) {
            ids.push_back(properties.m_layerID);
        }
    }

    return ids;
}

void CompositingManager::removeViewWithID(long viewID) {
    for (auto it = m_compositingViews.begin(); it != m_compositingViews.end(); it++) {
        if ((*it)->viewID() == viewID) {
            m_compositingViews.erase(it);
            m_idSummary -= viewID;
            return;
        }
    }
}

}