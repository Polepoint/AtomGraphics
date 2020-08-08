//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "TransactionProperties.h"
#include "PlatformLayerBackingStore.h"

namespace AtomGraphics {

TransactionProperties::TransactionProperties(const LayerProperties &layerProperties) {

    m_layerID = layerProperties.layerID;
    m_superlayerID = layerProperties.superlayerID;

    m_position = layerProperties.position;
    m_bounds = layerProperties.bounds;
    m_contentsScale = layerProperties.contentsScale;

    m_changedProperties = layerProperties.changedProperties;

    if ((m_changedProperties & BackingStoreChanged) && layerProperties.backingStoreAttached) {
#if PLATFORM(IOS)
        m_buffer = &layerProperties.backingStore->m_frontBuffer;
#elif PLATFORM(ANDROID)
        m_backingStoreImage = layerProperties.backingStore->m_frontBuffer.finishRecording();
#endif
    }
}

TransactionProperties::TransactionProperties(const TransactionProperties &other) {
    *this = other;
}

TransactionProperties &TransactionProperties::operator=(const TransactionProperties &other) {
    m_layerID = other.m_layerID;
    m_superlayerID = other.m_superlayerID;
    m_position = other.m_position;
    m_bounds = other.m_bounds;
    m_contentsScale = other.m_contentsScale;
    m_changedProperties = other.m_changedProperties;
#if PLATFORM(IOS)
    m_buffer = other.m_buffer;
#elif PLATFORM(ANDROID)
    m_backingStoreImage = other.m_backingStoreImage;
#endif

    return *this;
}

}
