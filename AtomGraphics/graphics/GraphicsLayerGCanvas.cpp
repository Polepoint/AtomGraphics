//
// Created by neo on 2018/7/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsLayerGCanvas.h"
#include "GraphicsLayerBackingStoreGCanvas.h"

namespace AtomGraphics {
    GraphicsLayerGCanvas::GraphicsLayerGCanvas(AtomGraphics::PlatformLayer *platformLayer, AtomGraphics::Node *rootNode)
            : GraphicsLayer(platformLayer, rootNode) {
        m_backingStore = new GraphicsLayerBackingStoreGCanvas(this);
    }
}
