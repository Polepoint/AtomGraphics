//
// Created by neo on 2018/4/19.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMCANVASNODECG_H
#define ATOMGRAPHICS_ATOMCANVASNODECG_H

#include "AtomCanvasNode.h"
#include "AtomLayerBackingStore.h"
#include "AtomContentFlushController.h"

namespace AtomGraphics {

    class CanvasNodeCG : public CanvasNode {

    public:
        CanvasNodeCG(AtomContentFlushController *m_backingStore);

        CanvasContext2d *getContext2d() override;

    protected:
        CanvasContext2d *createContext2d() override;

    private:
        AtomContentFlushController *m_flushController;
    };
}


#endif //ATOMGRAPHICS_ATOMCANVASNODECG_H
