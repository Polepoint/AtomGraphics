//
// Created by neo on 2018/7/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsLayerCG.h"
#import "GraphicsLayerBackingStoreCG.h"

namespace AtomGraphics {
    GraphicsLayerCG::GraphicsLayerCG(PlatformLayer *platformLayer, Node *rootNode)
            : GraphicsLayer(platformLayer, rootNode) {
        m_backingStore = new GraphicsLayerBackingStoreCG(this);
    }
}