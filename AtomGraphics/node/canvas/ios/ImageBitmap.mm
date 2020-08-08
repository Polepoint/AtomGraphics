//
// Created by woo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "ImageBitmap.h"
#include "ImageBuffer.h"

namespace AtomGraphics {

ImageBitmap::ImageBitmap() {
    m_imageBuffer = ImageBuffer::CreateEmptyImageBuffer();
    m_imageLoader = std::make_unique<ImageLoader>(*this);
}

ImageBitmap::~ImageBitmap() {
    m_imageBuffer->release();
}

float ImageBitmap::width() const {
    return m_imageBuffer->m_imageSize.width;
}

float ImageBitmap::height() const {
    return m_imageBuffer->m_imageSize.height;
}

void ImageBitmap::setWidth(float width) {
    m_imageBuffer->m_imageSize.width = static_cast<int>(width);
    m_imageBuffer->m_data->m_bufferImageSize.width = static_cast<int>(width);
    m_imageBuffer->m_data->m_bytesPerRow = static_cast<size_t>(width) * 4;
}

void ImageBitmap::setHeight(float height) {
    m_imageBuffer->m_imageSize.height = static_cast<int>(height);
    m_imageBuffer->m_data->m_bufferImageSize.height = static_cast<int>(height);
}

void ImageBitmap::setBuffer(const scoped_refptr<SharedBuffer> &buffer) {
    m_imageBuffer->m_data->m_buffer = buffer;
}

void ImageBitmap::setConfiguration(const ImageBitmapConfiguration &configuration) {
    m_configuration = configuration;
    m_imageBuffer->m_configuration = m_configuration;
}

ImageBuffer *ImageBitmap::imageBuffer() {
    return m_imageBuffer;
}

}