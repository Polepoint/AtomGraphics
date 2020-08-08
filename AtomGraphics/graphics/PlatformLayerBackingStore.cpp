//
// Created by neo on 2018/7/5.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "PlatformLayerBackingStore.h"
#include <algorithm>
#include <utility>
#include "GraphicsContextStateSaver.h"
#include "PlatformLayerBackingStorePainter.h"
#include "PlatformLayer.h"

#if PLATFORM(IOS)
#include "GraphicsContextCG.h"
#include "ImageBuffer.h"
#elif PLATFORM(ANDROID)

#include "graphics/android/skia/GraphicsContextSkia.h"

#endif

namespace AtomGraphics {

PlatformLayerBackingStore::PlatformLayerBackingStore(PlatformLayer *layerOwner) : m_layerOwner(layerOwner) {

}

IntSize PlatformLayerBackingStore::backingStoreSize() const {
    FloatSize scaledSize = m_size;
    scaledSize.scale(m_scale);
    return roundedIntSize(scaledSize);
}

void PlatformLayerBackingStore::ensureBackingStore(const FloatSize &size, float scale) {
    if (m_size == size && m_scale == scale)
        return;

    m_size = size;
    m_scale = scale;

    if (m_frontBuffer) {
        // If we have a valid backing store, we need to ensure that it gets completely
        // repainted the next time display() is called.
        setNeedsDisplay();
    }

    clearBackingStore();
}

void PlatformLayerBackingStore::clearBackingStore() {
    m_frontBuffer.discard();
    m_backBuffer.discard();
}

void PlatformLayerBackingStore::setNeedsDisplay() {
    FloatRect bounds(0, 0, m_size.width, m_size.height);
    setNeedsDisplayInRect(IntRect(bounds));
}

void PlatformLayerBackingStore::setNeedsDisplayInRect(const IntRect &rect) {
    m_dirtyRegion.unite(rect);
}

bool PlatformLayerBackingStore::display() {
    bool backingStoreChanged = false;

    IntSize expandedScaledSize = backingStoreSize();
    if (!m_dirtyRegion.isEmpty() && !expandedScaledSize.equals(IntSize::ZERO)) {

        std::swap(m_frontBuffer, m_backBuffer);

        if (!m_frontBuffer) {
            m_frontBuffer.reset(expandedScaledSize);
        }

        bool willPaintEntireBackingStore = m_dirtyRegion.contains(IntRect(IntPoint(), IntSize(m_size)));

#if PLATFORM(IOS)

        std::unique_ptr<GraphicsContextCG> context = m_frontBuffer.bitmapBuffer()->createGraphicsContext();

        if (m_backBuffer && !willPaintEntireBackingStore) {
            RetainPtr<CGImageRef> backImage = m_backBuffer.bitmapBuffer()->makeCGImage();
            CGContextRef cgContext = context->getCGContextRef();

            GraphicsContextStateSaver stateSaver(context.get());
            CGContextSetBlendMode(cgContext, kCGBlendModeCopy);
            CGContextTranslateCTM(cgContext, 0, expandedScaledSize.height);
            CGContextScaleCTM(cgContext, 1, -1);
            CGContextDrawImage(cgContext, CGRectMake(0, 0, expandedScaledSize.width, expandedScaledSize.height), backImage.get());
        }

#elif PLATFORM(ANDROID)

        SkCanvas *canvas = m_frontBuffer.recordingCanvas();
        auto context = std::unique_ptr<GraphicsContextSkia>(new GraphicsContextSkia(canvas));

        if (m_backBuffer && !willPaintEntireBackingStore) {

        }

#endif

        drawInContext(context.get());

        m_frontContextPendingFlush = std::move(context);

        backingStoreChanged = true;

    }

    return backingStoreChanged;
}


void PlatformLayerBackingStore::drawInContext(GraphicsContext *context) {
    IntRect dirtyBounds = m_dirtyRegion.bounds();
    std::vector<IntRect> dirtyRects = m_dirtyRegion.rects();

    static const unsigned webLayerMaxRectsToPaint = 5;
    constexpr static const float webLayerWastedSpaceThreshold = 0.75f;

    if (dirtyRects.size() > webLayerMaxRectsToPaint ||
            m_dirtyRegion.totalArea() > webLayerWastedSpaceThreshold * dirtyBounds.width() * dirtyBounds.height()) {
        dirtyRects.clear();
        dirtyRects.push_back(dirtyBounds);
    }

    for (const auto &rect : dirtyRects) {
        FloatRect scaledRect(rect);
        scaledRect.scale(m_scale);
        scaledRect = enclosingIntRect(scaledRect);
        scaledRect.scale(1 / m_scale);
        m_paintingRects.push_back(scaledRect);
    }

#if PLATFORM(IOS)

    CGRect cgPaintingRects[webLayerMaxRectsToPaint];
    for (size_t i = 0, dirtyRectCount = m_paintingRects.size(); i < dirtyRectCount; ++i) {
        FloatRect scaledPaintingRect = m_paintingRects[i];
        scaledPaintingRect.scale(m_scale);
        cgPaintingRects[i] = scaledPaintingRect;
    }

    CGContextRef cgContext = ((GraphicsContextCG *) context)->getCGContextRef();

    CGContextClipToRects(cgContext, cgPaintingRects, m_paintingRects.size());
    context->scale(m_scale, m_scale);

#elif PLATFORM(ANDROID)

    SkCanvas *canvas = ((GraphicsContextSkia *) context)->getSkCanvas();
    for (size_t i = 0, dirtyRectCount = m_paintingRects.size(); i < dirtyRectCount; ++i) {
        FloatRect scaledPaintingRect = m_paintingRects[i];
        scaledPaintingRect.scale(m_scale);
        canvas->clipRect(scaledPaintingRect);
    }
    canvas->scale(m_scale, m_scale);

#endif

    PlatformLayerBackingStorePainter::paintLayer(this, context, m_paintingRects);

    m_dirtyRegion = RectRegion();
    m_paintingRects.clear();
}

}