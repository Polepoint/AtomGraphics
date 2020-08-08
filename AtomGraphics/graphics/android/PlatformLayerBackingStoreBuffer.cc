//
// Created by neo on 2019/2/11.
//

#include "graphics/PlatformLayerBackingStoreBuffer.h"
#include <memory>
#include <skia/core/SkImage.h>
#include <skia/core/SkCanvas.h>

namespace AtomGraphics {

PlatformLayerBackingStoreBuffer::PlatformLayerBackingStoreBuffer(const PlatformLayerBackingStoreBuffer &other) {
    m_recorder = other.m_recorder;
}

PlatformLayerBackingStoreBuffer::~PlatformLayerBackingStoreBuffer() {

}

void PlatformLayerBackingStoreBuffer::discard() {
    if (m_recorder) {
        m_recorder->m_sk_recorder.finishRecordingAsPicture();
        m_recorder.reset();
        m_recordingCanvas = nullptr;
    }
}

void PlatformLayerBackingStoreBuffer::reset(IntSize size) {
    discard();
    m_recorder = std::make_shared<Recorder>();
    m_contentSize = size;
    m_recordingCanvas = m_recorder->m_sk_recorder.beginRecording(size.width, size.height);
}

SkCanvas *PlatformLayerBackingStoreBuffer::recordingCanvas() {
    return m_recordingCanvas;
}

sk_sp<SkImage> PlatformLayerBackingStoreBuffer::finishRecording() {
    auto pic = m_recorder->m_sk_recorder.finishRecordingAsPicture();
    m_recordingCanvas = m_recorder->m_sk_recorder.beginRecording(m_contentSize.width, m_contentSize.height);
    return SkImage::MakeFromPicture(pic,
                                    SkISize::Make(m_contentSize.width, m_contentSize.height),
                                    nullptr,
                                    nullptr,
                                    SkImage::BitDepth::kU8,
                                    SkColorSpace::MakeSRGB());
}

}