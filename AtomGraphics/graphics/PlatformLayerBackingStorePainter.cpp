//
// Created by neo on 2018/11/15.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "PlatformLayerBackingStorePainter.h"
#include "GraphicsContextStateSaver.h"

namespace AtomGraphics {

static std::vector<Node *> drawnNodes;

void PlatformLayerBackingStorePainter::buildBackingStoreTransactionBegin() {
    drawnNodes.clear();
}

void PlatformLayerBackingStorePainter::buildBackingStoreTransactionEnd() {
    for (auto node : drawnNodes) {
        node->didEndDrawingContent();
    }
}

void PlatformLayerBackingStorePainter::paintLayer(
        PlatformLayerBackingStore *backingStore,
        GraphicsContext *context,
        std::vector<IntRect> dirtyRects) {
    
    GraphicsLayer *graphicsLayer = backingStore->m_layerOwner->getGraphicsLayer();
    
    FloatRect backingStoreInLayerBounds = graphicsLayer->platformLayerFrameInGraphics(backingStore->m_layerOwner);

    context->save();
    context->translate(-backingStoreInLayerBounds.origin.x, -backingStoreInLayerBounds.origin.y);
    Node *rootNode = graphicsLayer->getRootNode().get();
    if (rootNode) {
        for (auto rect : dirtyRects) {
            context->save();
            FloatRect dirtyRectRelateToNode = rect;
            dirtyRectRelateToNode.origin.add(backingStoreInLayerBounds.origin);
            paintNodeInContext(rootNode, context, dirtyRectRelateToNode);
            context->restore();
        }
    }
    context->restore();
}

void PlatformLayerBackingStorePainter::paintNodeInContext(Node *node, GraphicsContext *context, const IntRect &bounds) {
    
    //draw background
    //draw border
    //draw content
    
    FloatRect nodeFrame = node->getFrame();
    
    if (!nodeFrame.intersectsRect(bounds)) {
        return;
    }
    
    context->clipRect(nodeFrame);
    
    bool nodeDrawn = std::find(drawnNodes.begin(), drawnNodes.end(), node) != drawnNodes.end();
    
    if (!nodeDrawn) {
        node->willDrawContent();
    }
    
    FloatRect contentBounds(nodeFrame);
    contentBounds.intersect(bounds);
    contentBounds.move(-nodeFrame.origin.x, -nodeFrame.origin.y);
    
    node->drawContent(context, contentBounds);
    
    if (!nodeDrawn) {
        drawnNodes.push_back(node);
    }
    
    auto children = node->getChildren();
    for (auto child : children) {
        //translate
        context->save();
        const FloatRect &frame = child->getFrame();
        context->translate(frame.origin.x, frame.origin.y);
        FloatRect translatedDirtyBounds(bounds);
        translatedDirtyBounds.origin.subtract(frame.origin);
        paintNodeInContext(child, context, translatedDirtyBounds);
        context->restore();
    }
}
    
}