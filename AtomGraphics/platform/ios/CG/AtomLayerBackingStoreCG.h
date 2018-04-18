//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AtomLayerBackingStore.h"
#import "AtomNode.h"

namespace AtomGraphics {

    class AtomLayerBackingStoreCG : public AtomLayerBackingStore {

    public:

        void applyBackingStoreToLayer(CALayer *layer);

        void flush();

        void paintContext();

    private:
        AtomGraphics::ImageBuffer *_frontBuffer;
        Node *_rootNode;
    };

}