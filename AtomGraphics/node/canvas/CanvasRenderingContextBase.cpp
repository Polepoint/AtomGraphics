//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "CanvasRenderingContextBase.h"

namespace AtomGraphics {


bool CanvasRenderingContextBase::is2D() {
    return false;
}

bool CanvasRenderingContextBase::isWebGL() {
    return false;
}
}