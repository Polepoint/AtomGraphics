//
// Created by neo on 2019/1/22.
//
#include "node/canvas/ImageBitmap.h"

namespace AtomGraphics {

ImageBitmap::ImageBitmap() {
    m_imageLoader = std::make_unique<ImageLoader>(*this);
}

ImageBitmap::~ImageBitmap() {

}

float ImageBitmap::width() const {
    return m_size.width;
}

float ImageBitmap::height() const {
    return m_size.height;
}

void ImageBitmap::setWidth(float width) {
    m_size.width = static_cast<int>(width);
}

void ImageBitmap::setHeight(float height) {
    m_size.height = static_cast<int>(height);
}

void ImageBitmap::setBuffer(const scoped_refptr<SharedBuffer> &buffer) {

    m_buffer = buffer;
    if (!m_buffer) {
        m_skImage.reset();
        return;
    }
    SkImageInfo imageInfo = SkImageInfo::MakeN32Premul(m_size);
    sk_sp<SkData> pixels = SkData::MakeWithoutCopy(
            buffer->data(),
            static_cast<size_t>(4 * m_size.width * m_size.height));
    m_skImage = SkImage::MakeRasterData(
            imageInfo, std::move(pixels),
            static_cast<size_t>(4 * m_size.width));
}

void ImageBitmap::setConfiguration(const ImageBitmapConfiguration &configuration) {
    m_configuration = configuration;
}

sk_sp<SkImage> ImageBitmap::skImage() {
    return m_skImage;
}

}
