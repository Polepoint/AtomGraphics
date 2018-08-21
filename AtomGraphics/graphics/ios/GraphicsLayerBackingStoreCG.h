//
// Created by neo on 2018/7/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORECG_H
#define ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORECG_H

#import "GraphicsLayerBackingStore.h"

namespace AtomGraphics {

    class GraphicsLayerBackingStoreCG : public GraphicsLayerBackingStore {

    public:
        GraphicsLayerBackingStoreCG(GraphicsLayer *graphicsLayer);

        void applyBackingStoreToLayer(PlatformLayer *layer) override;
    };

}

#endif //ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORECG_H
