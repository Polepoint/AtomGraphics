//
// Created by neo on 2018/4/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMLAYERBACKINGSTORE_H
#define ATOMLAYERBACKINGSTORE_H

#import "AtomGraphics.h"
#import "AtomImageBuffer.h"
#import <QuartzCore/CALayer.h>

namespace AtomGraphics {

    class AtomLayerBackingStore {

    public:
        virtual void applyBackingStoreToLayer(CALayer *layer) {
        };

        virtual void flush() {
        };
    };

}

#endif //ATOMLAYERBACKINGSTORE_H
