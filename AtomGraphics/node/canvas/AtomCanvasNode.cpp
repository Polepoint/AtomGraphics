//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasNode.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreGraphics/CGBitmapContext.h>

#endif

namespace AtomGraphics {

    CanvasContext2d *CanvasNode::createContext2d() {
        if (!_canvasContext2d) {
            _canvasContext2d = new CanvasContext2d(getContentSize());
        }

        return _canvasContext2d;
    }

    CanvasContext3d *CanvasNode::createContext3d() {
        return nullptr;
    }
}

