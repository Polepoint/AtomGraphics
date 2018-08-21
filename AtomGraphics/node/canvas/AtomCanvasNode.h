//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASNODE_H
#define ATOMCANVASNODE_H

#include "node/AtomNode.h"
#include "graphics/ImageBuffer.h"
#include "AtomCanvasContext3D.h"

typedef enum {
    CanvasContextType2D,
    CanvasContextTypeWebGL,
} CanvasContextType;

namespace AtomGraphics {

    class CanvasContext2D;

    class CanvasNode : public Node {

    public:

        virtual CanvasContext2D *getContext2d();
    };
}


#endif //ATOMCANVASNODE_H
