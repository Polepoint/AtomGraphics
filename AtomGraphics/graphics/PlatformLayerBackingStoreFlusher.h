//
// Created by neo on 2018/11/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYERBACKINGSTOREFLUSHER_H
#define ATOMGRAPHICS_PLATFORMLAYERBACKINGSTOREFLUSHER_H

#include <vector>
#include "memory/RefCounted.h"
#include "GraphicsContext.h"

namespace AtomGraphics {

class PlatformLayerBackingStoreFlusher : public RefCounted<PlatformLayerBackingStoreFlusher> {

public:

    std::vector<std::unique_ptr<GraphicsContext>> m_contexts;

    PlatformLayerBackingStoreFlusher(std::vector<std::unique_ptr<GraphicsContext>> contextList);

};

}


#endif //ATOMGRAPHICS_PLATFORMLAYERBACKINGSTOREFLUSHER_H
