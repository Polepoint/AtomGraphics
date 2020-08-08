//
// Created by neo on 2019/4/5.
//

#include "GraphicsContextSaverSkia.h"

namespace AtomGraphics {
GraphicsContextSaverSkia::GraphicsContextSaverSkia(GraphicsContextSkia *context)
        : m_context(context) {
    m_saveCount = context->saveCount();
}

GraphicsContextSaverSkia::~GraphicsContextSaverSkia() {
    m_context->restoreToSaveCount(m_saveCount);
}

}