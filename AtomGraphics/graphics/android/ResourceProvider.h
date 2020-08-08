//
// Created by neo on 2018/7/26.
//

#ifndef ATOMGRAPHICS_RESOURCEPROVIDER_H
#define ATOMGRAPHICS_RESOURCEPROVIDER_H

#include <skia/core/SkSurface.h>
#include <skia/gpu/GrContext.h>
#include <skia/core/SkPictureRecorder.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#include "gpu/GLThread.h"
#include "gpu/GLResource.h"
#include "graphics/android/skia/GraphicsContextSkia.h"

namespace AtomGraphics {

class ResourceProvider {

public:

    static std::unique_ptr<ResourceProvider> CreateResourceProvider(ANativeWindow *nativeWindow,
                                                                    bool accelerated,
                                                                    scoped_refptr<GLResource> resource = scoped_refptr<GLResource>());

    static std::unique_ptr<ResourceProvider> CreateOffscreenResourceProvider(IntSize size,
                                                                             bool accelerated,
                                                                             scoped_refptr<GLResource> resource = scoped_refptr<GLResource>());

    ~ResourceProvider();

    sk_sp<SkImage> makeImageSnapshot(AccelerationHint hint);

    void drawPicture(sk_sp<SkPicture> pic, const SkMatrix *matrix = nullptr, SkPaint *paint = nullptr);

    void drawImage(sk_sp<SkImage> image, int x, int y);

    bool writePixels(const SkImageInfo &orig_info, const void *pixels, size_t row_bytes, int x, int y);

    void playback(sk_sp<SkPicture> pic);

    void swapBuffers();

    bool isAccelerated() const {
        return m_isAccelerated;
    }

private:

    struct RenderTaskDataBundle {

        RenderTaskDataBundle(ResourceProvider *owner) : m_owner(owner) {}

        sk_sp<SkSurface> skSurface();

        sk_sp<SkSurface> m_skSurface;
        ResourceProvider *m_owner;
    };

    const IntSize m_bufferSize;
    bool m_isAccelerated{false};
    bool m_isOffscreen;

    scoped_refptr<GLResource> m_glResource{nullptr};

    RenderTaskDataBundle *m_renderTaskDataBundle;

    ResourceProvider(IntSize size,
                     bool accelerated,
                     bool isOffscreen,
                     scoped_refptr<GLResource> resource);

    void executeOrPostAsyncRenderTask(std::function<void()> task);

    void executeOrPostSyncRenderTask(std::function<void()> task);

    sk_sp<SkSurface> createSkSurface();


    friend RenderTaskDataBundle;
};
}


#endif //ATOMGRAPHICS_RESOURCEPROVIDER_H
