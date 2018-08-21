//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsContextCG.h"


namespace AtomGraphics {

    void GraphicsContextCG::flush() {
        CGContextFlush(m_context);
    }

    CGContextRef GraphicsContextCG::getCGContextRef() {
        return m_context;
    }
}