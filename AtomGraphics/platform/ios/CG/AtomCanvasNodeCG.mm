//
// Created by neo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasNodeCG.h"
#include "AtomCanvasContext2dCG.h"

namespace AtomGraphics {

    CanvasNodeCG::CanvasNodeCG(AtomContentFlushController *m_backingStore) : m_flushController(m_backingStore) {
        _canvasContext2d = createContext2d();
    }

    CanvasContext2d *CanvasNodeCG::getContext2d() {
        _canvasContext2d->ensureDrawingContext();
        return _canvasContext2d;
    }

    CanvasContext2d *CanvasNodeCG::createContext2d() {
        return new CanvasContext2dCG(m_flushController);
    }

}