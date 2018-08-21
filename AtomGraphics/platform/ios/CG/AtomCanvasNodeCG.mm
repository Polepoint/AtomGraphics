//
// Created by neo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasNodeCG.h"
#include "AtomCanvasContext2dCG.h"

namespace AtomGraphics {

    CanvasNodeCG::CanvasNodeCG() : _canvasContext2d(new CanvasContext2dCG(this)) {

    }

    CanvasContext2D *CanvasNodeCG::getContext2d() {
        _canvasContext2d->ensureDrawingContext();
        return _canvasContext2d;
    }

    void CanvasNodeCG::draw(GraphicsContext *context) {

    }

}