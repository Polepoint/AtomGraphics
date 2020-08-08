//
// Created by neo on 2018/10/6.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYERBACKINGSTOREBUFFER_H
#define ATOMGRAPHICS_PLATFORMLAYERBACKINGSTOREBUFFER_H


#include "math/AtomGeometry.h"

#if PLATFORM(IOS)

#include "ImageBuffer.h"

#elif PLATFORM(ANDROID)

#include <skia/core/SkPictureRecorder.h>

#endif

namespace AtomGraphics {

class PlatformLayerBackingStoreBuffer {

#if PLATFORM(ANDROID)

    struct Recorder {

        Recorder() = default;

        SkPictureRecorder m_sk_recorder;
    };

#endif

public:

    PlatformLayerBackingStoreBuffer() {}

    PlatformLayerBackingStoreBuffer(const PlatformLayerBackingStoreBuffer &other);

    ~PlatformLayerBackingStoreBuffer();

    void discard();

    void reset(IntSize size);

#if PLATFORM(IOS)

    ImageBuffer *bitmapBuffer() const { return m_bitmapBuffer; }

#elif PLATFORM(ANDROID)

    SkCanvas *recordingCanvas();

    sk_sp<SkImage> finishRecording();

#endif

    explicit operator bool() const {
#if PLATFORM(IOS)
        return m_bitmapBuffer != nullptr;
#elif PLATFORM(ANDROID)
        return m_recordingCanvas != nullptr;
#endif

    }

private:

#if PLATFORM(IOS)
    ImageBuffer *m_bitmapBuffer{nullptr};
#elif PLATFORM(ANDROID)
    IntSize m_contentSize;
    std::shared_ptr<Recorder> m_recorder{nullptr};
    SkCanvas *m_recordingCanvas{nullptr};
#endif

};

}

#endif //ATOMGRAPHICS_PLATFORMLAYERBACKINGSTOREBUFFER_H
