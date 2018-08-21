//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASCONTEXTBASE_H
#define ATOMCANVASCONTEXTBASE_H

namespace AtomGraphics {

    class CanvasContextBase {

    public:
        virtual bool is2D();

        virtual bool isWebGL();
    };
}

#endif //ATOMCANVASCONTEXTBASE_H
