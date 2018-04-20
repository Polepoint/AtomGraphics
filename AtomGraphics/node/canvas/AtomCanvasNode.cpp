//
// Created by neo on 2018/4/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasNode.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreGraphics/CGBitmapContext.h>

#endif

namespace AtomGraphics {

    void CanvasNode::setContentSize(const Size &contentSize) {
        Node::setContentSize(contentSize);
        _canvasContext2d->setContentSize(contentSize);
    }

    CanvasContext2d *CanvasNode::getContext2d() {
        _canvasContext2d->ensureDrawingContext();
        return _canvasContext2d;
    }

    void CanvasNode::draw(GraphicsContext *context, Painter *painter) {
        _canvasContext2d->drawConsuming(context, Rect(_position.x, _position.y, _contentSize.width, _contentSize.height));
    }

    CanvasNode::CanvasNode() {
        _canvasContext2d = new CanvasContext2d();
    }

    CanvasContext2d *CanvasNode::createContext2d() {
        return new CanvasContext2d();
    }

    CanvasContextGL *CanvasNode::createContextGL() {
        return nullptr;
    }
}

