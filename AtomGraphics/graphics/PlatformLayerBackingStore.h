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

#include "base/AtomTypes.h"
#include "RectRegion.h"
#include "GraphicsContext.h"
#include "PlatformLayerBackingStoreBuffer.h"

namespace AtomGraphics {

class PlatformLayer;

class PlatformLayerBackingStore final {

public:

    PlatformLayerBackingStore(PlatformLayer *layerOwner);

    void ensureBackingStore(const FloatSize &size, float scale);

    void setNeedsDisplay();

    void setNeedsDisplayInRect(const IntRect &rect);

    /**
     * 将layer染到Buffers上
     */
    virtual bool display();

    std::unique_ptr<GraphicsContext> takeFrontContextPendingFlush() {
        return std::move(m_frontContextPendingFlush);
    };

private:

    PlatformLayer *m_layerOwner{nullptr};

    RectRegion m_dirtyRegion;
    FloatSize m_size{FloatSize::ZERO};
    float m_scale{1};

    std::unique_ptr<GraphicsContext> m_frontContextPendingFlush;
    std::vector<IntRect> m_paintingRects;


    PlatformLayerBackingStoreBuffer m_frontBuffer;
    PlatformLayerBackingStoreBuffer m_backBuffer;


    virtual void clearBackingStore();

    IntSize backingStoreSize() const;

    void drawInContext(GraphicsContext *context);

    friend class PlatformLayerBackingStorePainter;

    friend class TransactionProperties;

};

}


#endif //ATOMGRAPHICS_GRAPHICSLAYERBACKINGSTORE_H
