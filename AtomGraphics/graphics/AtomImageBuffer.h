//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMIMAGEBUFFER_H
#define ATOMIMAGEBUFFER_H

#include "AtomGraphicsContext.h"

namespace AtomGraphics {

    class ImageBuffer {

        void *getBufferData();

        void *data;

        int backingStoreWidth;
        int backingStoreHeight;

        PlatformContext context;

    private:

    };

}

#endif //ATOMIMAGEBUFFER_H
