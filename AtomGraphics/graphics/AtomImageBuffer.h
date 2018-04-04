//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMIMAGEBUFFER_H
#define ATOMIMAGEBUFFER_H

#include "AtomGraphicsContext.h"

namespace AtomGraphics {

    class ImageBuffer {

        void * getBufferData();

    private:
        PlatformContext _context;
    };

}

#endif //ATOMIMAGEBUFFER_H
