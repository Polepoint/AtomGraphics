//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICSCONTEXT_H
#define ATOMGRAPHICSCONTEXT_H

/**
 * GraphicsContext
 * 简单而言，iOS上和CoreGraphics的Context的APIs差不多
 * Android上和SkCanvas的APIs差不多
 */

namespace AtomGraphics {

    class GraphicsContext {

    public:
        /**
         * triggers execution of all pending draw operations
         */
        virtual void flush() {

        };
    };

}

#endif //ATOMGRAPHICSCONTEXT_H
