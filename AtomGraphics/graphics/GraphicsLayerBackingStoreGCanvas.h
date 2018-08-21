//
// Created by neo on 2018/7/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTOREGCANVAS_H
#define ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTOREGCANVAS_H

#include "GraphicsLayerBackingStore.h"

namespace AtomGraphics {

    class GraphicsLayerBackingStoreGCanvas: public GraphicsLayerBackingStore {

    public:
        GraphicsLayerBackingStoreGCanvas(GraphicsLayer *graphicsLayer);

        void applyBackingStoreToLayer(PlatformLayer *layer) override;

        void display() override;


    private:
        std::string m_pendingFlushCommand;
        int m_targetContexType;
    };

}


#endif //ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTOREGCANVAS_H
