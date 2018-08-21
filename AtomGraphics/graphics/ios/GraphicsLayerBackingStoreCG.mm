//
// Created by neo on 2018/7/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsLayerBackingStoreCG.h"
#import "PlatformLayerCA.h"
#import "GraphicsContextCG.h"

namespace AtomGraphics {

    GraphicsLayerBackingStoreCG::GraphicsLayerBackingStoreCG(AtomGraphics::GraphicsLayer *graphicsLayer)
            : GraphicsLayerBackingStore(graphicsLayer) {
    }

    void GraphicsLayerBackingStoreCG::applyBackingStoreToLayer(PlatformLayer *layer) {
        PlatformLayerCA *platformLayerCA = static_cast<PlatformLayerCA *>(layer);
        CALayer *caLayer = platformLayerCA->caLayer();
        GraphicsContextCG *graphicsContext = dynamic_cast<GraphicsContextCG *>(m_imageBuffer->createGraphicsContext());
        CGImageRef image = CGBitmapContextCreateImage(graphicsContext->getCGContextRef());
        caLayer.contents = (__bridge id) image;
    }

}
