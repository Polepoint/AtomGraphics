//
// Created by neo on 2018/7/5.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsLayerBackingStore.h"

namespace AtomGraphics {

    GraphicsLayerBackingStore::GraphicsLayerBackingStore(GraphicsLayer *graphicsLayer) : m_layer(graphicsLayer) {

    }

    void GraphicsLayerBackingStore::display() {
        //TODO: prepare context
        //构建Context，让layer渲染到当前context上
        if (m_layer && m_imageBuffer) {
            GraphicsContext *context = m_imageBuffer->createGraphicsContext();
            m_layer->drawLayerContents(context);
            m_frontContextPendingFlush = context;
        }
    }
}