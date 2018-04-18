//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomLayerBackingStoreCG.h"

namespace AtomGraphics {

    void AtomLayerBackingStoreCG::applyBackingStoreToLayer(CALayer *layer) {
        CGImageRef image = CGBitmapContextCreateImage(_frontBuffer->createGraphicsContext());
        layer.contents = (__bridge id) image;
    }

    void AtomLayerBackingStoreCG::flush() {
        CGContextFlush(_frontBuffer->createGraphicsContext());
    }

    void AtomLayerBackingStoreCG::paintContext() {
        CGContextRef context = _frontBuffer->createGraphicsContext();
        GraphicsContext *graphicsContext = new GraphicsContext(context);
        _rootNode->draw(graphicsContext, nullptr);
    }
}