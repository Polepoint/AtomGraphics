//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGraphicsContext.h"
#include "AtomPainter_iOSCoreGraphic.h"

namespace atomgraphics {

    PlatformContext GraphicsContext::platformContext() const {
        return _platformContext;
    }

    GraphicsContext::GraphicsContext(PlatformContext platformContext) {
        _platformContext = platformContext;
        _painter = new Painter_iOSCoreGraphic(this);
    }

}