//
// Created by neo on 2019-08-15.
//

#ifndef ATOMGRAPHICS_GLFUNCTOR_H
#define ATOMGRAPHICS_GLFUNCTOR_H

#include <cstdint>
#include <jni.h>
#include "graphics/AffineTransform.h"
#include "math/IntRect.h"
#include "base/AtomMacros.h"
#include "android/spi/draw_gl.h"
#include "android/spi/DrawGlInfo.h"
#include "AtomGraphicsJNIHelper.h"
#include "ContentView.h"

typedef int32_t status_t;

namespace android {

class Functor {

public:
    Functor() {}

    virtual ~Functor() {}

    virtual status_t operator()(int /*what*/, void * /*data*/) { return 0; }
};

}

namespace AtomGraphics {

class GLFunctor : public android::Functor {

public:

    GLFunctor(JNIEnv *env, jobject obj, ContentView *contentView) {
        mRef = env->NewGlobalRef(obj);
        mContentView = contentView;
    }

    virtual status_t operator()(int what, void *data) {
        AwDrawGLInfo aw_info;
        aw_info.version = kAwDrawGLInfoVersion;
        switch (what) {
            case DrawGlInfo::kModeDraw: {
                aw_info.mode = AwDrawGLInfo::kModeDraw;
                DrawGlInfo *gl_info = reinterpret_cast<DrawGlInfo *>(data);
                // Map across the input values.
                aw_info.clip_left = gl_info->clipLeft;
                aw_info.clip_top = gl_info->clipTop;
                aw_info.clip_right = gl_info->clipRight;
                aw_info.clip_bottom = gl_info->clipBottom;
                aw_info.width = gl_info->width;
                aw_info.height = gl_info->height;
                aw_info.is_layer = gl_info->isLayer;

                for (int i = 0; i < NELEM(aw_info.transform); ++i) {
                    aw_info.transform[i] = gl_info->transform[i];
                }

                break;
            }
            case DrawGlInfo::kModeProcess:
                aw_info.mode = AwDrawGLInfo::kModeProcess;
                break;
            case DrawGlInfo::kModeProcessNoContext:
                aw_info.mode = AwDrawGLInfo::kModeProcessNoContext;
                break;
            case DrawGlInfo::kModeSync:
                aw_info.mode = AwDrawGLInfo::kModeSync;
                break;
            default:
                return DrawGlInfo::kStatusDone;
        }

        drawGLFunction(&aw_info);

        return DrawGlInfo::kStatusDone;
    }


    void destroy() {
        JNIEnv *env = AtomGraphicsBase::AttachCurrentThread();
        env->DeleteGlobalRef(mRef);
        mContentView = nullptr;
        delete this;
    }


    void setContentView(ContentView *contentView) {
        mContentView = contentView;
    }

private:

    jobject mRef;
    ContentView *mContentView;
    IntRect mViewPort;
    AffineTransform mTransform;

    void drawGLFunction(AwDrawGLInfo *draw_info);

    void updateContentViewPort(IntRect viewPort, AffineTransform transform);

};

}


#endif //ATOMGRAPHICS_GLFUNCTOR_H
