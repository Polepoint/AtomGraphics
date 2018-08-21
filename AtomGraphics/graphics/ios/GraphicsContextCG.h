//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSCONTEXTGC_H
#define ATOMGRAPHICS_GRAPHICSCONTEXTGC_H

#import "GraphicsContext.h"
#import <CoreGraphics/CGContext.h>

namespace AtomGraphics {

    class GraphicsContextCG : public GraphicsContext {

    public:
        GraphicsContextCG(CGContextRef platformContext) : m_context(platformContext) {

        }

        CGContextRef getCGContextRef();


        void flush() override;

    private:
        CGContextRef m_context;
    };
}


#endif //ATOMGRAPHICS_GRAPHICSCONTEXTGC_H
