//
// Created by neo on 2019-08-15.
//

#include "GLFunctor.h"

namespace AtomGraphics {

void GLFunctor::drawGLFunction(AwDrawGLInfo *draw_info) {
    switch (draw_info->mode) {
        case AwDrawGLInfo::kModeSync:
        case AwDrawGLInfo::kModeProcessNoContext:
        case AwDrawGLInfo::kModeProcess:
            break;
        case AwDrawGLInfo::kModeDraw: {
            AffineTransform transform(draw_info->transform[0], draw_info->transform[4],
                                      draw_info->transform[2], draw_info->transform[5],
                                      draw_info->transform[12], draw_info->transform[13]);

            IntRect viewPort(draw_info->clip_left,
                             draw_info->clip_top,
                             draw_info->clip_right - draw_info->clip_left,
                             draw_info->clip_bottom - draw_info->clip_top);

            if (viewPort != mViewPort || transform != mTransform) {
                this->updateContentViewPort(viewPort, transform);
            }
            break;
        }
    }
}

void GLFunctor::updateContentViewPort(IntRect viewPort, AffineTransform transform) {
    if (!mContentView) {
        return;
    }

    mViewPort = viewPort;
    mTransform = transform;
    AffineTransform inverseTransform;
    if (transform.inverse(inverseTransform)) {
        IntRect contentBounds = inverseTransform.mapRect(mViewPort);
        mContentView->updateVisibleBounds(
                contentBounds.x(), contentBounds.y(),
                contentBounds.width(), contentBounds.height());
    }
}

}
