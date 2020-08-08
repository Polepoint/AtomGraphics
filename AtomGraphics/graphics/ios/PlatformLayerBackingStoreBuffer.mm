//
// Created by neo on 2018/10/6.
//

#include "PlatformLayerBackingStoreBuffer.h"

namespace AtomGraphics {

PlatformLayerBackingStoreBuffer::PlatformLayerBackingStoreBuffer(const PlatformLayerBackingStoreBuffer &other) {
    m_bitmapBuffer = other.m_bitmapBuffer;
    if (m_bitmapBuffer) {
        m_bitmapBuffer->retain();
    }
}

PlatformLayerBackingStoreBuffer::~PlatformLayerBackingStoreBuffer() {
    discard();
}

void PlatformLayerBackingStoreBuffer::discard() {
    if (m_bitmapBuffer) {
        m_bitmapBuffer->release();
        m_bitmapBuffer = nullptr;
    }
}

void PlatformLayerBackingStoreBuffer::reset(IntSize size) {
    discard();
    m_bitmapBuffer = ImageBuffer::Create(size, 4);
}

}