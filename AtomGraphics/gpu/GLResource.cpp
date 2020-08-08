//
// Created by neo on 2019/3/30.
//

#include <GLES2/gl2.h>
#include <skia/gpu/gl/GrGLInterface.h>
#include <android/native_window.h>
#include "base/AtomLog.h"
#include "GLResource.h"
#include "GLInterface.h"

namespace AtomGraphics {

GLResource::GLResource(ANativeWindow *nativeWindow)
        : m_nativeWindow(nativeWindow), m_glThread(new GLThread) {
    ANativeWindow_acquire(m_nativeWindow);
    m_windowWidth = ANativeWindow_getWidth(nativeWindow);
    m_windowHeight = ANativeWindow_getHeight(nativeWindow);
}

void GLResource::updateWindow(ANativeWindow *nativeWindow) {
    m_glThread->postAsyncRenderTask([this, nativeWindow]() -> void {
        if (m_nativeWindow != nativeWindow) {
            ANativeWindow_release(m_nativeWindow);
            ANativeWindow_acquire(nativeWindow);
            m_nativeWindow = nativeWindow;
            m_windowWidth = ANativeWindow_getWidth(nativeWindow);
            m_windowHeight = ANativeWindow_getHeight(nativeWindow);
        }

        if (m_eglSurface) {
            eglDestroySurface(m_eglDisplay, m_eglSurface);
            eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            initializeEGLSurface();
        }
    });
}

void GLResource::destroy() {
    if (!m_grContext) {
        return;
    }
    auto grContext = std::move(m_grContext);
    auto nativeWindow = m_nativeWindow;
    auto eglDisplay = m_eglDisplay;
    auto eglContext = m_eglContext;
    auto eglSurface = m_eglSurface;

    m_glThread->postAsyncRenderTask([nativeWindow, grContext, eglDisplay, eglContext, eglSurface]() -> void {
        grContext->freeGpuResources();
        ANativeWindow_release(nativeWindow);
        eglDestroyContext(eglDisplay, eglContext);
        eglDestroySurface(eglDisplay, eglSurface);
        eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglTerminate(eglDisplay);
        eglReleaseThread();
    });

    m_glThread->destroy();
    m_glThread = nullptr;
}

GLThread *GLResource::getGLThread() {
    return m_glThread;
}

sk_sp<GrContext> GLResource::grContext() {
    if (!m_grContext) {
        initializeEGL();
        auto glInterface = GLInterface::MakeDebugInterface(true);
        m_grContext = GrContext::MakeGL(std::move(glInterface));
    }

    return m_grContext;
}

void GLResource::initializeEGL() {
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_eglDisplay != EGL_NO_DISPLAY) {

        if (!eglInitialize(m_eglDisplay, nullptr, nullptr)) {
            AtomLogError("eglInitialize failed!");
            return;
        }

        EGLint config_attribs_8888[] = {EGL_ALPHA_SIZE, 8,
                                        EGL_BLUE_SIZE, 8,
                                        EGL_GREEN_SIZE, 8,
                                        EGL_RED_SIZE, 8,
                                        EGL_STENCIL_SIZE, 8,
                                        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                                        EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
                                        EGL_NONE};

        EGLint *choose_attributes = config_attribs_8888;
        EGLint num_configs;

        if (!eglChooseConfig(m_eglDisplay, choose_attributes, &m_eglConfig, 1, &num_configs)) {
            AtomLogError("eglChooseConfig error :%i", eglGetError());
            return;
        }

        if (m_eglConfig) {
            initializeEGLContext();
            initializeEGLSurface();
        }
    }
}

void GLResource::initializeEGLContext() {
    static const EGLint kEGLContextAttribsForOpenGLES[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, nullptr, kEGLContextAttribsForOpenGLES);

    glClearStencil(0);
    glClearColor(0, 0, 0, 0);
    glStencilMask(0xffffffff);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void GLResource::initializeEGLSurface() {
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_nativeWindow, nullptr);

    if (m_eglSurface && m_eglSurface != EGL_NO_SURFACE) {
        eglSurfaceAttrib(m_eglDisplay, m_eglSurface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
    } else {
        EGLint error = eglGetError();
        AtomLogError("failed to create eglSurface! Error: 0x%x", error);
        return;
    }

    if (m_nativeWindow) {
        eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
    }
}

}
