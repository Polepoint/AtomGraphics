//
// Created by neo on 2018/11/21.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_LAYERCHANGE_H
#define ATOMGRAPHICS_LAYERCHANGE_H

#include <cstdint>

namespace AtomGraphics {

enum LayerChange {
    NoChange                    = 0,
    ChildrenChanged             = 1LLU << 1,
    DrawsContentChanged         = 1LLU << 2,
    PositionChanged             = 1LLU << 3,
    BoundsChanged               = 1LLU << 4,
    ContentsScaleChanged        = 1LLU << 5,
    CoverageRectChanged         = 1LLU << 6,
    TilingAreaChanged           = 1LLU << 7,
    BackingStoreChanged         = 1LLU << 8
    
};

typedef uint64_t LayerChangeFlags;

}


#endif //ATOMGRAPHICS_LAYERCHANGE_H
