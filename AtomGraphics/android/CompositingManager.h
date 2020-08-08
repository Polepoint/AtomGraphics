//
// Created by neo on 2019/2/13.
//

#ifndef ATOMGRAPHICS_COMPOSITINGMANAGER_H
#define ATOMGRAPHICS_COMPOSITINGMANAGER_H

#include "graphics/Transaction.h"
#include "CompositingView.h"

namespace AtomGraphics {

class CompositingManager {

public:

    std::shared_ptr<CompositingView> createViewWithID(long viewID);

    std::shared_ptr<CompositingView> viewWithID(long viewID);

    std::vector<long> subviewIDs(long superviewID, const Transaction &transaction);

    void removeViewWithID(long viewID);

    const std::vector<std::shared_ptr<CompositingView>> &compositingViews() const {
        return m_compositingViews;
    }

    long idSummary() const {
        return m_idSummary;
    }

private:

    long m_idSummary{0};
    std::vector<std::shared_ptr<CompositingView>> m_compositingViews;
};

}


#endif //ATOMGRAPHICS_COMPOSITINGMANAGER_H
