//
// Created by neo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_IMAGEBUFFERCG_H
#define ATOMGRAPHICS_IMAGEBUFFERCG_H

#import "ImageBuffer.h"
#import  <CoreGraphics/CoreGraphics.h>

namespace AtomGraphics {

    class ImageBufferCG : public ImageBuffer {

    public:

        ImageBufferCG(size_t width, size_t height, size_t bytesPerRow);

        GraphicsContext *createGraphicsContext() override;

    private:
        CGDataProviderRef m_DataProvider;

        void releaseBitmapContextData(void *typelessBitmap, void *typelessData);
    };

}

#endif //ATOMGRAPHICS_IMAGEBUFFERCG_H
