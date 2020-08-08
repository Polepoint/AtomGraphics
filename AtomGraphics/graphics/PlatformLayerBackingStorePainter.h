//
// Created by neo on 2018/11/15.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYERBACKINGSTORERENDERER_H
#define ATOMGRAPHICS_PLATFORMLAYERBACKINGSTORERENDERER_H

#include "GraphicsLayer.h"
#include "PlatformLayerBackingStore.h"
#include "GraphicsContext.h"
#include "node/Node.h"
#include <vector>

namespace AtomGraphics {

class PlatformLayerBackingStorePainter {

public:
    
    static void buildBackingStoreTransactionBegin();
    
    static void buildBackingStoreTransactionEnd();
    
    static void paintLayer(PlatformLayerBackingStore *backingStore, GraphicsContext *context, std::vector<IntRect> dirtyRects);

private:
    
    static void paintNodeInContext(Node *node, GraphicsContext *context, const IntRect &bounds);
    
};


}


#endif //ATOMGRAPHICS_PLATFORMLAYERBACKINGSTORERENDERER_H
