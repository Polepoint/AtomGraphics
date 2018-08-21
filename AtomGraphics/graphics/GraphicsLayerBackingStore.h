//
// Created by neo on 2018/7/5.
// Copyright (c) 2018 neo. All rights reserved.
//

/**
 * 作为layer内容的的暂存
 * 在替换旧的的layer视图之前, 要逐一构建新视图的layer图形, 并且存储起来
 * 当layer内容发生变化时, 需要将变化的地方记录下来, 当flush的时候再统一执行变化
 */

#ifndef ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORE_H
#define ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORE_H

#include "GraphicsLayer.h"
#include "PlatformLayer.h"
#include "ImageBuffer.h"

namespace AtomGraphics {

    class GraphicsLayer;

    class GraphicsLayerBackingStore {

    public:

        GraphicsLayerBackingStore(GraphicsLayer *graphicsLayer);

        virtual void applyBackingStoreToLayer(PlatformLayer *layer) {
        };

        /**
         * 将layer染到Buffers上
         */
        virtual void display();

        GraphicsContext *takePendingFlushContext() {
            return m_frontContextPendingFlush;
        };

    protected:
        GraphicsLayer *m_layer{nullptr};
        ImageBuffer *m_imageBuffer{nullptr};
        GraphicsContext *m_frontContextPendingFlush{nullptr};
    };
}


#endif //ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORE_H
