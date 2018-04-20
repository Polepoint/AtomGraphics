//
// Created by neo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMIMAGEBUFFERCG_H
#define ATOMGRAPHICS_ATOMIMAGEBUFFERCG_H

#include "AtomImageBuffer.h"

namespace AtomGraphics {

    class ImageBufferCG : public ImageBuffer {

    public:

        ImageBufferCG(size_t width, size_t height, size_t bytesPerRow);

        PlatformContext createGraphicsContext() override;

        CGDataProviderRef m_DataProvider;

        void releaseBitmapContextData(void *typelessBitmap, void *typelessData);
    };

}

#endif //ATOMGRAPHICS_ATOMIMAGEBUFFERCG_H
