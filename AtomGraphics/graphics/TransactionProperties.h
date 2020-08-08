//
// Created by neo on 2018/11/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TRANSACTIONPROPERTIES_H
#define ATOMGRAPHICS_TRANSACTIONPROPERTIES_H

#include "LayerProperties.h"
#include "LayerChange.h"

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#include "PlatformLayerBackingStoreBuffer.h"

#elif PLATFORM(ANDROID)

#include <skia/core/SkPicture.h>

#endif

namespace AtomGraphics {

class TransactionProperties {

public:

    TransactionProperties(const LayerProperties &layerProperties);

    TransactionProperties(const TransactionProperties &other);

    TransactionProperties &operator=(const TransactionProperties &other);

    LayerChangeFlags m_changedProperties{NoChange};

    long m_layerID{layerIDNone};
    long m_superlayerID{layerIDNone};

    FloatPoint m_position;
    FloatRect m_bounds;
    float m_contentsScale;

#if PLATFORM(IOS)
    PlatformLayerBackingStoreBuffer *m_buffer{nullptr};
#elif PLATFORM(ANDROID)
    sk_sp<SkImage> m_backingStoreImage;
#endif

};

}

#endif //ATOMGRAPHICS_TRANSACTIONPROPERTIES_H
