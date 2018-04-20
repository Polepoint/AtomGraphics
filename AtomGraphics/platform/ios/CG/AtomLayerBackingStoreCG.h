//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomLayerBackingStore.h"
#import "AtomNode.h"
#import "AtomImageBufferCG.h"

namespace AtomGraphics {

    class AtomLayerBackingStoreCG : public AtomLayerBackingStore {

    public:

        AtomLayerBackingStoreCG(Node *_rootNode);

        void applyBackingStoreToLayer(CALayer *layer);

        void flush() override;

        void paintContext() override;

        void setContentSize(Size _contentSize);

    private:
        ImageBufferCG *_frontBuffer;
        Node *_rootNode;
        Size _contentSize;
    };

}