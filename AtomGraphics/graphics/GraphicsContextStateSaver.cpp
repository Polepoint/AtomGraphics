//
// Created by neo on 2018/8/26.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsContextStateSaver.h"

namespace AtomGraphics {

GraphicsContextStateSaver::GraphicsContextStateSaver(GraphicsContext *context, bool doSave) : m_context(context) {
    if (doSave) {
        save();
    }
}

GraphicsContextStateSaver::~GraphicsContextStateSaver() {
    if (m_saved) {
        m_context->restore();
    }
}

void GraphicsContextStateSaver::save() {
    if (!m_saved) {
        m_context->save();
        m_saved = true;
    }
}

void GraphicsContextStateSaver::restore() {
    if (m_saved) {
        m_context->restore();
        m_saved = false;
    }
}

}