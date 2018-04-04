//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICSCONTEXT_H
#define ATOMGRAPHICSCONTEXT_H

#include "AtomPlatformConfig.h"
#include "AtomPainter.h"


#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#import <CoreGraphics/CGContext.h>

typedef CGContextRef PlatformContext;

#endif

namespace AtomGraphics {

    class Painter;

    class GraphicsContext {

    public:
        PlatformContext platformContext() const;

        GraphicsContext(PlatformContext platformContext);

    private:
        /**
         *     Canvas/CoreGraphic/OpenGl/Metal
         */
        PlatformContext _platformContext;
        Painter *_painter;
    };

}

#endif //ATOMGRAPHICSCONTEXT_H
