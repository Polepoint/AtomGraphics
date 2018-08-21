//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasNode.h"
#include "AtomCanvasContext2D.h"
#include "platform/AtomPlatformConfig.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreGraphics/CGBitmapContext.h>

#endif

namespace AtomGraphics {

    CanvasContext2D *CanvasNode::getContext2d() {
        return nullptr;
    }

}

