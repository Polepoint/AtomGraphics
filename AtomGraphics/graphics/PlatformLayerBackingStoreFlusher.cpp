//
// Created by neo on 2018/11/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "PlatformLayerBackingStoreFlusher.h"


namespace AtomGraphics {

PlatformLayerBackingStoreFlusher::PlatformLayerBackingStoreFlusher(std::vector<std::unique_ptr<GraphicsContext>> contextList)
        : m_contexts(std::move(contextList)) {
    
}

}