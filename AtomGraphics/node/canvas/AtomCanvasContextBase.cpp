//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasContextBase.h"

namespace AtomGraphics {


    bool CanvasContextBase::is2D() {
        return false;
    }

    bool CanvasContextBase::isWebGL() {
        return false;
    }
}