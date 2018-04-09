//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomImageBuffer.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreGraphics/CGBitmapContext.h>

#endif

namespace AtomGraphics {

    void *ImageBuffer::getBufferData() {
        if (!context) {
            return nullptr;
        }

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS
        return CGBitmapContextGetData(context);
#endif

    }

}