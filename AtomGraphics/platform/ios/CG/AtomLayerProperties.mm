//
// Created by neo on 2018/4/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomLayerProperties.h"

namespace AtomGraphics {

    uint64_t AtomLayerProperties::getLayerID() const {
        return _layerID;
    }

    AtomLayerBackingStore *AtomLayerProperties::getLayerBackingStore() {
        return _layerBackingStore;
    }

    bool AtomLayerProperties::isLayerBackingStoreChanged() {
        return _layerBackingStoreChanged;
    }

    void AtomLayerProperties::setLayerBackingStoreChanged(bool changed) {
        _layerBackingStoreChanged = changed;
    }
}