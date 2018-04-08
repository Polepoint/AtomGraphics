//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASNODE_H
#define ATOMCANVASNODE_H

#include "AtomNode.h"
#include "AtomImageBuffer.h"
#include "AtomCanvasContext2d.h"
#include "AtomCanvasContext3d.h"

namespace AtomGraphics {

    class CanvasNode : public Node {

    public:
        CanvasNode();

        CanvasContext2d *getContext2d();

        void draw(GraphicsContext *context, Painter *painter) override;

        void setContentSize(const Size &contentSize) override;

    private:

        CanvasContext2d *createContext2d();

        CanvasContext3d *createContext3d();

        CanvasContext2d *_canvasContext2d;
        CanvasContext3d *_canvasContext3d;
    };
}


#endif //ATOMCANVASNODE_H
