//
// Created by neo on 2018/12/25.
//

#include <skia/gpu/GrBackendSurface.h>
#include <skia/gpu/gl/GrGLInterface.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "gpu/GLResourceManager.h"
#include "javascript/android/JSPageScopeTracer.h"
#include "javascript/android/JSPageScope.h"
#include "base/AtomLog.h"
#include "ResourceProvider.h"
#include "math/IntRect.h"
#include "math/IntSize.h"
#include "math/IntPoint.h"
#include "math/FloatSize.h"
#include "graphics/Image.h"
#include "graphics/ImageData.h"
#include "graphics/AffineTransform.h"

namespace AtomGraphics {

std::unique_ptr<ResourceProvider> ResourceProvider::CreateResourceProvider(ANativeWindow *nativeWindow,
                                                                           bool accelerated,
                                                                           scoped_refptr<GLResource> resource) {
    return std::unique_ptr<ResourceProvider>(
            new ResourceProvider(
                    IntSize(ANativeWindow_getWidth(nativeWindow), ANativeWindow_getHeight(nativeWindow)),
                    accelerated, false, std::move(resource)));
}

std::unique_ptr<ResourceProvider> ResourceProvider::CreateOffscreenResourceProvider(IntSize size,
                                                                                    bool accelerated,
                                                                                    scoped_refptr<GLResource> resource) {
    if (size.isEmpty()) {
        size.setSize(1, 1);
    }

    return std::unique_ptr<ResourceProvider>(new ResourceProvider(size, accelerated, true, std::move(resource)));
}

ResourceProvider::ResourceProvider(IntSize size,
                                   bool accelerated,
                                   bool isOffscreen,
                                   scoped_refptr<GLResource> resource)
        : m_bufferSize(size),
          m_isAccelerated(accelerated),
          m_isOffscreen(isOffscreen),
          m_glResource(std::move(resource)),
          m_renderTaskDataBundle(new RenderTaskDataBundle(this)) {
}


ResourceProvider::~ResourceProvider() {
    RenderTaskDataBundle *dataBundle = m_renderTaskDataBundle;
    dataBundle->m_owner = nullptr;
    auto task = [dataBundle]() {
        delete dataBundle;
    };

    executeOrPostAsyncRenderTask(std::move(task));
}

void ResourceProvider::executeOrPostAsyncRenderTask(std::function<void()> task) {
    if (m_isAccelerated) {
        if (!m_glResource) {
            return;
        }
        GLThread *glThread = m_glResource->getGLThread();
        if (glThread)
            glThread->postAsyncRenderTask(std::move(task));
    } else {
        task();
    }
}

void ResourceProvider::executeOrPostSyncRenderTask(std::function<void()> task) {
    if (m_isAccelerated) {
        if (!m_glResource) {
            return;
        }
        GLThread *glThread = m_glResource->getGLThread();
        if (glThread)
            glThread->postSyncRenderTask(std::move(task));
    } else {
        task();
    }
}

sk_sp<SkImage> ResourceProvider::makeImageSnapshot(AccelerationHint hint) {
    sk_sp<SkImage> snapshot;
    RenderTaskDataBundle *dataBundle = m_renderTaskDataBundle;
    auto task = [dataBundle, &snapshot, hint]() {
        auto surface = dataBundle->skSurface();
        surface->flush();
        sk_sp<SkImage> newSnapshot = hint == kPreferAcceleration ?
                                     surface->makeImageSnapshot() :
                                     surface->makeImageSnapshot()->makeRasterImage();
        snapshot.swap(newSnapshot);
    };

    executeOrPostSyncRenderTask(std::move(task));

    return snapshot;
}

void ResourceProvider::drawPicture(sk_sp<SkPicture> pic, const SkMatrix *matrix, SkPaint *paint) {
    std::unique_ptr<SkMatrix> ctm = std::unique_ptr<SkMatrix>(matrix ? new SkMatrix(*matrix) : nullptr);
    std::unique_ptr<SkPaint> p = std::unique_ptr<SkPaint>(paint ? new SkPaint(*paint) : nullptr);
    RenderTaskDataBundle *dataBundle = m_renderTaskDataBundle;
    auto task = [dataBundle, pic, &p, &ctm]() {
        dataBundle->skSurface()->getCanvas()->drawPicture(pic, ctm.get(), p.get());
    };

    executeOrPostAsyncRenderTask(std::move(task));
}

void ResourceProvider::drawImage(sk_sp<SkImage> image, int x, int y) {
    RenderTaskDataBundle *dataBundle = m_renderTaskDataBundle;
    auto task = [dataBundle, image, x, y]() {
        dataBundle->skSurface()->getCanvas()->drawImage(image, x, y);
    };

    executeOrPostAsyncRenderTask(std::move(task));
}

bool ResourceProvider::writePixels(const SkImageInfo &orig_info, const void *pixels, size_t row_bytes, int x, int y) {

    RenderTaskDataBundle *dataBundle = m_renderTaskDataBundle;
    auto task = [dataBundle, orig_info, pixels, row_bytes, x, y]() {
        auto surface = dataBundle->skSurface();
        if (!surface->getCanvas()->writePixels(orig_info, pixels, row_bytes, x, y)) {
            //TODO: Log error
        }
    };

    executeOrPostAsyncRenderTask(std::move(task));

    return true;
}

void ResourceProvider::playback(sk_sp<SkPicture> pic) {
    RenderTaskDataBundle *dataBundle = m_renderTaskDataBundle;
    auto task = [dataBundle, pic]() {
        pic->playback(dataBundle->skSurface()->getCanvas());
    };

    executeOrPostAsyncRenderTask(std::move(task));
}

void ResourceProvider::swapBuffers() {
    if (!m_isAccelerated || m_isOffscreen) {
        return;
    }

    GLResource *glResource = this->m_glResource.get();
    auto task = [glResource]() {
        if (glResource->getEglDisplay() && glResource->getEglContext() && glResource->getEglSurface()) {
            if (!eglSwapBuffers(glResource->getEglDisplay(), glResource->getEglSurface())) {
                //TODO: log error
            }
        }
    };

    executeOrPostAsyncRenderTask(std::move(task));
}

sk_sp<SkSurface> ResourceProvider::RenderTaskDataBundle::skSurface() {
    if (!m_skSurface && m_owner) {
        m_skSurface = m_owner->createSkSurface();
    }

    return m_skSurface;
}

sk_sp<SkSurface> ResourceProvider::createSkSurface() {
    if (m_isAccelerated) {
        sk_sp<GrContext> context = m_glResource->grContext();
        if (m_isOffscreen) {
            SkSurfaceProps surfaceProps(0, kUnknown_SkPixelGeometry);
            const SkImageInfo info = SkImageInfo::Make(
                    m_bufferSize.width, m_bufferSize.height,
                    kN32_SkColorType,
                    kPremul_SkAlphaType, SkColorSpace::MakeSRGB());

            return SkSurface::MakeRenderTarget(
                    context.get(), SkBudgeted::kNo, info,
                    kBottomLeft_GrSurfaceOrigin, &surfaceProps);
        } else {
            SkSurfaceProps surfaceProps = SkSurfaceProps(0, SkSurfaceProps::kLegacyFontHost_InitType);
            GrGLFramebufferInfo framebufferInfo;
            framebufferInfo.fFBOID = 0;
            framebufferInfo.fFormat = GL_RGBA8_OES;
            GrBackendRenderTarget renderTarget(m_bufferSize.width,
                                               m_bufferSize.height,
                                               0,
                                               8,
                                               framebufferInfo);

            return SkSurface::MakeFromBackendRenderTarget(
                    context.get(), renderTarget, kBottomLeft_GrSurfaceOrigin,
                    kRGBA_8888_SkColorType, nullptr, &surfaceProps);
        }
    } else {
        SkImageInfo info = SkImageInfo::Make(
                m_bufferSize.width, m_bufferSize.height,
                kN32_SkColorType, kPremul_SkAlphaType);
        return SkSurface::MakeRaster(info);
    }
}

}