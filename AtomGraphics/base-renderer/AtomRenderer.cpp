//
// Created by neo on 2018/3/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomRenderer.h"

namespace AtomGraphics {

    void Renderer::render(Node *node, GraphicsContext *context) {
//        node->draw(context);

        auto &children = node->getChildren();
        for (const auto &child : children) {
            render(child, context);
        }
    }

}
