//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMIMAGEBUFFER_H
#define ATOMIMAGEBUFFER_H

#include "GraphicsContext.h"
#include <cstddef>

namespace AtomGraphics {

    class ImageBuffer {

    public:

        virtual GraphicsContext* createGraphicsContext(){
            return nullptr;
        };

    protected:
        void *m_data;

        size_t m_bytesPerRow;
        size_t m_backingStoreHeight;
        size_t m_backingStoreWidth;


    };

}

#endif //ATOMIMAGEBUFFER_H
