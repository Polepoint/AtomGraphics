//
// Created by neo on 2019/3/30.
//

#ifndef ATOMGRAPHICS_GPURESOURCE_H
#define ATOMGRAPHICS_GPURESOURCE_H

#include <EGL/egl.h>
#include <skia/gpu/GrContext.h>
#include <atomic>
#include "GLThread.h"
#include "memory/RefCounted.h"

namespace AtomGraphics {

class GLResource : public RefCounted<GLResource> {

public:

    GLResource(ANativeWindow *);

    void updateWindow(ANativeWindow *);

    void destroy();

    GLThread *getGLThread();

    int getWindowWidth() const {
        return m_windowWidth;
    }

    int getWindowHeight() const {
        return m_windowHeight;
    }

    sk_sp<GrContext> grContext();

    EGLDisplay getEglDisplay() const {
        return m_eglDisplay;
    }

    EGLContext getEglContext() const {
        return m_eglContext;
    }

    EGLSurface getEglSurface() const {
        return m_eglSurface;
    }

private:

    ANativeWindow *m_nativeWindow = nullptr;
    sk_sp<GrContext> m_grContext;
    EGLDisplay m_eglDisplay{EGL_NO_DISPLAY};
    EGLConfig m_eglConfig{0};
    EGLContext m_eglContext{EGL_NO_CONTEXT};
    EGLSurface m_eglSurface{EGL_NO_SURFACE};

    int m_windowWidth;
    int m_windowHeight;

    GLThread *m_glThread;

    void initializeEGLContext();

    void initializeEGLSurface();

    void initializeEGL();
};

}


#endif //ATOMGRAPHICS_GPURESOURCE_H
