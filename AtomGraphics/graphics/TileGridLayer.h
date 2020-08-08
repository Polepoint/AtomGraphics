//
// Created by neo on 2018/11/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TILEGRIDLAYER_H
#define ATOMGRAPHICS_TILEGRIDLAYER_H

#include "math/AtomGeometry.h"
#include "PlatformLayer.h"

namespace AtomGraphics {

class TileGrid;

class TileGridLayer : public PlatformLayer {

public:

    TileGridLayer() : PlatformLayer() {}

    ~TileGridLayer() override = default;

    void setOwner(TileGrid *owner) {
        m_owner = owner;
    }

private:

    TileGrid *m_owner{nullptr};
};

}

#endif //ATOMGRAPHICS_TILEGRIDLAYER_H
