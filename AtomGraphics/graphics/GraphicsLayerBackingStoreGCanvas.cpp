//
// Created by neo on 2018/7/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsLayerBackingStoreGCanvas.h"
#include "node/canvas/AtomGCanvasNode.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

#include <graphics/android/PlatformLayerTextureView.h>

#endif

namespace AtomGraphics {


    GraphicsLayerBackingStoreGCanvas::GraphicsLayerBackingStoreGCanvas(GraphicsLayer *graphicsLayer)
            : GraphicsLayerBackingStore(graphicsLayer) {
    }

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

    void GraphicsLayerBackingStoreGCanvas::applyBackingStoreToLayer(PlatformLayer *layer) {
        PlatformLayerTextureView *textureView = static_cast<PlatformLayerTextureView *>(layer);
        textureView->renderCommand(m_pendingFlushCommand.data());
    }

#endif

    void GraphicsLayerBackingStoreGCanvas::display() {
        GCanvasNode *node = dynamic_cast<GCanvasNode *>(m_layer->getRootNode());
        GCanvasContext2D *context2d = dynamic_cast<GCanvasContext2D *>(node->getContext2d());
        m_pendingFlushCommand = context2d->flushCommands();
    }

}