//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASNODE_H
#define ATOMCANVASNODE_H

#include "AtomNode.h"
#include "AtomImageBuffer.h"
#include "AtomCanvasContext2d.h"
#include "AtomCanvasContextGL.h"

namespace AtomGraphics {

    class CanvasNode : public Node {

    public:

        virtual CanvasContext2d *getContext2d();

        void draw(GraphicsContext *context, Painter *painter) override;

        void setContentSize(const Size &contentSize) override;

        CanvasNode();

    protected:

        CanvasContext2d *createContext2d();

        CanvasContextGL *createContextGL();

        CanvasContext2d *_canvasContext2d;
        CanvasContextGL *_canvasContextGL;
    };
}


#endif //ATOMCANVASNODE_H
