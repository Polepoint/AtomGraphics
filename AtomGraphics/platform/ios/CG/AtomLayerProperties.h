//
// Created by neo on 2018/4/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMLAYERPROPERTIES_H
#define ATOMLAYERPROPERTIES_H

#include "AtomLayerBackingStore.h"

namespace AtomGraphics {

    class AtomLayerProperties {


    public:

        uint64_t getLayerID() const;

        AtomLayerBackingStore *getLayerBackingStore();

        bool isLayerBackingStoreChanged();

        void setLayerBackingStoreChanged(bool changed);

    private:
        uint64_t _layerID;
        AtomLayerBackingStore *_layerBackingStore;
        bool _layerBackingStoreChanged;
    };

}


#endif //ATOMLAYERPROPERTIES_H
