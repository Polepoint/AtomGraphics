//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomRenderer.h"

namespace atomgraphics {

    void Renderer::render(Node *node, GraphicsContext *context) {


        auto &children = node->getChildren();
        for (const auto &child : children) {
            render(child, context);
        }
    }

}
