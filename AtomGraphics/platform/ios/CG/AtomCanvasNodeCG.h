//
// Created by neo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMCANVASNODECG_H
#define ATOMGRAPHICS_ATOMCANVASNODECG_H

#include "AtomCanvasNode.h"
#import "AtomCanvasContext2dCG.h"

namespace AtomGraphics {

    class CanvasNodeCG : public CanvasNode {

    public:
        CanvasNodeCG();

        CanvasContext2D *getContext2d() override;

        void draw(GraphicsContext *context) override;

    private:
        CanvasContext2dCG *_canvasContext2d;
    };
}


#endif //ATOMGRAPHICS_ATOMCANVASNODECG_H
