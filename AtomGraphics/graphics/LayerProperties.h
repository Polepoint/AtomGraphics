//
// Created by neo on 2018/11/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_LAYERPROPERTY_H
#define ATOMGRAPHICS_LAYERPROPERTY_H

#include "math/AtomGeometry.h"
#include "LayerChange.h"
#include "PlatformLayerBackingStore.h"

namespace AtomGraphics {

static const long layerIDNone = 0;

struct LayerProperties {
    
    long layerID{layerIDNone};
    long superlayerID{layerIDNone};
    
    FloatPoint position;
    FloatRect bounds;
    float contentsScale{1};
    
    bool backingStoreAttached{false};
    std::unique_ptr<PlatformLayerBackingStore> backingStore;
    
    LayerChangeFlags changedProperties{NoChange};
    
    LayerProperties() {}
    
    LayerProperties(const LayerProperties &other) {
        layerID = other.layerID;
        superlayerID = other.superlayerID;
        position = other.position;
        bounds = other.bounds;
        contentsScale = other.contentsScale;
        changedProperties = other.changedProperties;
    }
    
    void notePropertiesChanged(LayerChange changeFlags) {
        changedProperties = changedProperties | changeFlags;
    }
    
    void reset() {
        changedProperties = NoChange;
    }
};

}

#endif //ATOMGRAPHICS_LAYERPROPERTY_H
