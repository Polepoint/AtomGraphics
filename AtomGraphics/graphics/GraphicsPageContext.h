//
// Created by neo on 2018/7/30.
//

#ifndef ATOMGRAPHICS_GRAPHICSPAGECONTEXT_H
#define ATOMGRAPHICS_GRAPHICSPAGECONTEXT_H

/**
 * share data among the pages with the same context
 */

#include "platform/AtomPlatformConfig.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

#include <jni.h>

#endif

#include "GraphicsContentFlushController.h"

namespace AtomGraphics {

    class GraphicsContentFlushController;

    class GraphicsPageContext {

    public:
#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

        GraphicsPageContext(jobject jContext);

        jobject getJContext() const;

#endif

        GraphicsContentFlushController *flushController();

    private:
        GraphicsContentFlushController *m_flushController{nullptr};
#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID
        jobject jContext;
#endif
    };
}


#endif //ATOMGRAPHICS_GRAPHICSPAGECONTEXT_H
