//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "TransactionApplier.h"
#import "CompositingView.h"
#import "CompositingManager.h"


namespace AtomGraphics {

void TransactionApplier::applyTransaction(Transaction *transaction, CompositingManager *manager) {
    const std::vector<TransactionProperties> &propertyList = transaction->getPropertyList();

    std::vector<long> referredProperties;
    long idSum = 0;
    for (const TransactionProperties &properties : propertyList) {
        applyTransactionProperties(properties, manager, transaction);
        referredProperties.push_back(properties.m_layerID);
        idSum += properties.m_layerID;
    }

    if (idSum != manager.idSum) {//a quick way to compare view tree changes
        NSArray *viewList = [manager viewList];
        if (propertyList.size() < viewList.count) {
            for (CompositingView *view in viewList) {
                if (std::find(referredProperties.begin(), referredProperties.end(), view.viewID) == referredProperties.end()) {
                    [manager removeViewWithID:view.viewID];
                }
            }
        }
    }
}

void TransactionApplier::applyTransactionProperties(const TransactionProperties &properties, CompositingManager *manager, Transaction *transaction) {

    CompositingView *compositingView = [manager useViewWithID:properties.m_layerID];

    LayerChangeFlags flags = properties.m_changedProperties;

    if (!compositingView) {
        compositingView = [manager createViewWithID:properties.m_layerID];
        auto superview = [manager useViewWithID:properties.m_superlayerID];
        if (superview) {
            [superview addSubview:compositingView];
        }
        flags = ~NoChange;
    }

    if (flags & PositionChanged) {
        compositingView.layer.position = CGPointMake(properties.m_position.x, properties.m_position.y);
    }

    if (flags & BoundsChanged) {
        compositingView.layer.bounds = CGRectMake(
                properties.m_bounds.origin.x,
                properties.m_bounds.origin.y,
                properties.m_bounds.size.width,
                properties.m_bounds.size.height);
    }

    if (flags & ContentsScaleChanged) {
        compositingView.layer.contentsScale = properties.m_contentsScale;
    }

    if (flags & BackingStoreChanged) {
        ImageBuffer *buffer = properties.m_buffer->bitmapBuffer();
        if (buffer->accelerated()) {
#if ENABLE_ACCELERATION
            if (buffer->surface()) {
                compositingView.layer.contents = buffer->surface()->asLayerContents();
            }
#endif
        } else {
            RetainPtr<CGImageRef> image = buffer->makeCGImage();
            compositingView.layer.contents = (__bridge id) image.get();
        }
    }

    if (flags & ChildrenChanged) {

        NSArray *subviewIDs = [manager subviewIDsOfViewWithID:properties.m_layerID fromTransaction:transaction];
        for (NSNumber *idNumber in subviewIDs) {
            CompositingView *view = [manager useViewWithID:[idNumber longValue]];
            if (view) {
                if (view.superview == compositingView) {
                    continue;
                }

                if (view.superview) {
                    [view removeFromSuperview];
                }
                [compositingView addSubview:view];
            }
        }

        if ([subviewIDs count] < compositingView.subviews.count) {
            NSArray *subviews = compositingView.subviews;
            for (CompositingView *subview in subviews) {
                if (![subviewIDs containsObject:@(subview.viewID)]) {
                    [subview removeFromSuperview];
                }
            }
        }
    }
}

}