//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMRENDERER_H
#define ATOMRENDERER_H


#include "AtomNode.h"
#include "AtomGraphicsContext.h"

namespace atomgraphics {

    class Node;

    class Renderer {
    public:
        static void render(Node *node, GraphicsContext *context);
    };
}
#endif //ATOMRENDERER_H
