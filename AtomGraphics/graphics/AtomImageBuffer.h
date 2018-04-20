//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMIMAGEBUFFER_H
#define ATOMIMAGEBUFFER_H

#include "AtomGraphicsContext.h"

namespace AtomGraphics {

    class ImageBuffer {

    public:

        virtual PlatformContext createGraphicsContext();

        void *getBufferData();

        void *m_data;

        size_t m_bytesPerRow;
        size_t m_backingStoreHeight;
        size_t m_backingStoreWidth;

        PlatformContext context;

    private:

    };

}

#endif //ATOMIMAGEBUFFER_H
