//
// Created by neo on 2018/7/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSLAYERGL_H
#define ATOMGRAPHICS_GRAPHICSLAYERGL_H

#include "GraphicsLayer.h"

namespace AtomGraphics{

    class GraphicsLayerGCanvas :public GraphicsLayer{

    public:
        GraphicsLayerGCanvas(PlatformLayer *platformLayer, Node *rootNode);
    };
}



#endif //ATOMGRAPHICS_GRAPHICSLAYERGL_H
