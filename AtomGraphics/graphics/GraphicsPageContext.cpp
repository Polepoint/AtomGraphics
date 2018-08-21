//
// Created by neo on 2018/7/30.
//

#include "GraphicsPageContext.h"

namespace AtomGraphics {

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

    GraphicsPageContext::GraphicsPageContext(jobject context) : jContext(context) {}

    jobject GraphicsPageContext::getJContext() const {
        return jContext;
    }

#endif

    GraphicsContentFlushController *GraphicsPageContext::flushController() {
        if (!m_flushController) {
            m_flushController = new GraphicsContentFlushController(this);
        }

        return m_flushController;
    }
}