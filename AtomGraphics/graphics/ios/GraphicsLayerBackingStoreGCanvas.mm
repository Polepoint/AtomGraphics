//
// Created by neo on 2018/7/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsLayerBackingStoreGCanvas.h"
#import "PlatformLayerCAEAGL.h"


namespace AtomGraphics {

    void GraphicsLayerBackingStoreGCanvas::applyBackingStoreToLayer(PlatformLayer *layer) {
        PlatformLayerCAEAGL *platformLayerCAEAGL = static_cast<PlatformLayerCAEAGL *>(layer);
        AtomGCanvasEAGLLayer *glLayer = platformLayerCAEAGL->glLayer();
        [glLayer renderCommand:[NSString stringWithUTF8String:m_pendingFlushCommand.data()]];
    }
}