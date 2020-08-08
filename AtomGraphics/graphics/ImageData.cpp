//
// Created by neo on 2018/12/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <cstdlib>
#include "ImageData.h"

namespace AtomGraphics {

scoped_refptr<ImageData> ImageData::Create(const IntSize &imageSize, size_t bytesPerRow) {
    return Create(
            imageSize,
            bytesPerRow,
            calloc(static_cast<size_t>(imageSize.height * imageSize.width * 4), sizeof(uint8_t)),
            false);
}

scoped_refptr<ImageData> ImageData::Create(const IntSize &imageSize,
                                             size_t bytesPerRow,
                                             void *data,
                                             bool withoutCopy) {
    ImageData *imageDataPtr;
    if (withoutCopy) {
        imageDataPtr = new ImageData(imageSize, bytesPerRow, data);
    } else {
        size_t bytesCount = imageSize.height * bytesPerRow;
        void *copiedData = malloc(bytesCount);
        memcpy(copiedData, data, bytesCount);
        imageDataPtr = new ImageData(imageSize, bytesPerRow, copiedData);
    }

    scoped_refptr<ImageData> imageData(imageDataPtr);
    imageData->deref();
    return imageData;
}

ImageData::ImageData(const IntSize &imageSize, size_t bytesPerRow, void *dataPtr)
        : m_imageSize(imageSize), m_bytesPerRow(bytesPerRow), m_dataPtr(dataPtr) {}

ImageData::~ImageData() {
    if (m_dataPtr) {
        free(m_dataPtr);
    }
}

}
