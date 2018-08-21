//
// Created by neo on 2018/7/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSLAYERCG_H
#define ATOMGRAPHICS_GRAPHICSLAYERCG_H

#import "GraphicsLayer.h"

namespace AtomGraphics {
    class GraphicsLayerCG :public GraphicsLayer{

    public:
        GraphicsLayerCG(PlatformLayer *platformLayer, Node *rootNode);
    };
}


#endif //ATOMGRAPHICS_GRAPHICSLAYERCG_H
