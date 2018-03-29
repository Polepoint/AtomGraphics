//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICSCONTEXT_H
#define ATOMGRAPHICSCONTEXT_H

#include "AtomPlatformConfig.h"


#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#import <CoreGraphics/CGContext.h>

typedef CGContextRef PlatformContext;

#endif

class GraphicsContext {

public:
    PlatformContext platformContext() const;

private:
    /**
     *     Canvas/CoreGraphic/OpenGl/Metal
     */
    PlatformContext _platformContext;
};


#endif //ATOMGRAPHICSCONTEXT_H
