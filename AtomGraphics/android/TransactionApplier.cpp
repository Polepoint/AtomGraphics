//
// Created by neo on 2019/2/13.
//

#include "TransactionApplier.h"


namespace AtomGraphics {

void TransactionApplier::applyTransaction(const Transaction &transaction, CompositingManager *manager) {
    const std::vector<TransactionProperties> &propertyList = transaction.getPropertyList();

    std::vector<long> referredProperties;
    long idSum = 0;
    for (const TransactionProperties &properties : propertyList) {
        applyTransactionProperties(properties, manager, transaction);
        referredProperties.push_back(properties.m_layerID);
        idSum += properties.m_layerID;
    }

    if (idSum != manager->idSummary()) {
        auto viewList = manager->compositingViews();
        if (propertyList.size() < viewList.size()) {
            for (auto view : viewList) {
                if (std::find(referredProperties.begin(), referredProperties.end(), view->viewID())
                        == referredProperties.end()) {
                    manager->removeViewWithID(view->viewID());
                }
            }
        }
    }
}

void TransactionApplier::applyTransactionProperties(const TransactionProperties &properties,
                                                    CompositingManager *manager,
                                                    const Transaction &transaction) {

    auto compositingView = manager->viewWithID(properties.m_layerID);

    LayerChangeFlags flags = properties.m_changedProperties;

    if (!compositingView) {
        compositingView = manager->createViewWithID(properties.m_layerID);
        auto superview = manager->viewWithID(properties.m_superlayerID);
        if (superview) {
            superview->addSubview(compositingView);
        }
        flags = ~NoChange;
    }

    if (flags & PositionChanged) {
        compositingView->setPosition(properties.m_position);
    }

    if (flags & BoundsChanged) {
        compositingView->setBounds(properties.m_bounds);
    }

    if (flags & ContentsScaleChanged) {
        compositingView->setContentsScale(properties.m_contentsScale);
    }

    if (flags & BackingStoreChanged) {
        compositingView->displayImage(properties.m_backingStoreImage);
    }

    if (flags & ChildrenChanged) {

        std::vector<long> subviewIDs = manager->subviewIDs(properties.m_layerID, transaction);
        for (auto viewID : subviewIDs) {
            std::shared_ptr<CompositingView> view = manager->viewWithID(viewID);
            if (view) {
                if (view->superview() == compositingView.get()) {
                    continue;
                }

                if (view->superview()) {
                    view->removeFromSuperView();
                }
                compositingView->addSubview(view);
            }
        }

        if (subviewIDs.size() < compositingView->subviews().size()) {
            auto subviews = compositingView->subviews();
            for (auto subview : subviews) {
                auto it = std::find(subviewIDs.begin(), subviewIDs.end(), subview->viewID());
                if (it == subviewIDs.end()) {
                    subview->removeFromSuperView();
                }
            }
        }
    }
}

}