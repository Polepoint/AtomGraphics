//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomLayerBackingStoreCG.h"

namespace AtomGraphics {

    AtomLayerBackingStoreCG::AtomLayerBackingStoreCG(Node *rootNode) : _rootNode(rootNode) {

    }

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

    void AtomLayerBackingStoreCG::setContentSize(Size contentSize) {
        _contentSize = contentSize;
        _frontBuffer = new ImageBufferCG(static_cast<size_t>(contentSize.width),
                static_cast<size_t>(contentSize.height),
                static_cast<size_t>(4 * contentSize.width));
    }
}