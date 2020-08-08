//
// Created by neo on 2018/12/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_IMAGEDATA_H
#define ATOMGRAPHICS_IMAGEDATA_H

#include <stddef.h>
#include "math/IntSize.h"
#include "memory/RefCounted.h"

namespace AtomGraphics {

class ImageData :public RefCounted<ImageData> {

public:

REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE();

    static scoped_refptr<ImageData> Create(const IntSize &imageSize, size_t bytesPerRow);

    static scoped_refptr<ImageData>
    Create(const IntSize &imageSize, size_t bytesPerRow, void *data, bool withoutCopy);

    ~ImageData();

    const IntSize &size() const {
        return m_imageSize;
    }

    size_t bytesCount() const {
        return m_bytesPerRow * m_imageSize.height;
    }

    int width() {
        return m_imageSize.width;
    }

    int height() {
        return m_imageSize.height;
    }

    void *data() const {
        return m_dataPtr;
    }

private:

    ImageData(const IntSize &imageSize, size_t bytesPerRow, void *dataPtr);

    IntSize m_imageSize;
    size_t m_bytesPerRow;
    void *m_dataPtr{nullptr};

    friend RefCounted;
};


}

#endif //ATOMGRAPHICS_IMAGEDATA_H
