//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMCANVASNODE_H
#define ATOMCANVASNODE_H

#include "AtomNode.h"
#include "AtomImageBuffer.h"

namespace AtomGraphics {

    class CanvasNode : public Node {

    public:

    private:
        ImageBuffer *_imageBuffer;
    };
}


#endif //ATOMCANVASNODE_H
