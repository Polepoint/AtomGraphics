//
// Created by neo on 2018/4/6.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASCONTEXTGL_H
#define ATOMCANVASCONTEXTGL_H

#include "CanvasRenderingContextBase.h"

namespace AtomGraphics {

class WebGLRenderingContext : public CanvasRenderingContextBase {

public:
    bool isWebGL() override;

};
}

#endif //ATOMCANVASCONTEXTGL_H
