//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomGCanvasNode.h"
#include "AtomGCanvasContext2D.h"

namespace AtomGraphics {

    GCanvasNode::GCanvasNode() : m_gcanvasContext2d(new GCanvasContext2D(this)) {

    }

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

    static jclass classObject = nullptr;

    jclass GCanvasNode::getClassJObject() {
        return classObject;
    }

    void GCanvasNode::setClassJObject(jclass classObj) {
        classObject = classObj;
    }

#endif

    CanvasContext2D *GCanvasNode::getContext2d() {
        m_currentContextType = CanvasContextType2D;
        return m_gcanvasContext2d;
    }

    void GCanvasNode::draw(GraphicsContext *context) {

    }

}
