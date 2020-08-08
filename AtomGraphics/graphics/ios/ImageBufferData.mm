//
// Created by neo on 2018/12/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <Accelerate/Accelerate.h>
#import "ImageBufferData.h"


namespace AtomGraphics {


static void unpremultiplyBufferData(const vImage_Buffer &src, const vImage_Buffer &dest) {

    if (kvImageNoError != vImageUnpremultiplyData_RGBA8888(&src, &dest, kvImageNoFlags))
        return;

    //TODO remove ?
    // Swap channels 1 and 3, to convert BGRA<->RGBA. IOSurfaces are BGRA, ImageData expects RGBA.
    const uint8_t map[4] = {2, 1, 0, 3};
    vImagePermuteChannels_ARGB8888(&dest, &dest, map, kvImageNoFlags);
}


static void premultiplyBufferData(const vImage_Buffer &src, const vImage_Buffer &dest) {

    if (kvImageNoError != vImagePremultiplyData_RGBA8888(&src, &dest, kvImageNoFlags))
        return;

    //TODO remove ?
    // Swap channels 1 and 3, to convert BGRA<->RGBA. IOSurfaces are BGRA, ImageData expects RGBA.
    const uint8_t map[4] = {2, 1, 0, 3};
    vImagePermuteChannels_ARGB8888(&dest, &dest, map, kvImageNoFlags);
}

ImageBufferData::ImageBufferData(const IntSize &bufferSize, size_t bytesPerRow)
        : m_bufferImageSize(bufferSize), m_bytesPerRow(bytesPerRow) {
    size_t bytesCount = static_cast<size_t>(bufferSize.height * bytesPerRow);
    if (bytesCount > 0) {
        void *dataPtr = calloc(bytesCount, sizeof(uint8_t));
        m_buffer = MakeRefCounted<SharedBuffer>(dataPtr, bytesCount);
    }
}

ImageBufferData::ImageBufferData(void *data, const IntSize &bufferSize, size_t bytesPerRow)
        : m_bufferImageSize(bufferSize), m_bytesPerRow(bytesPerRow) {
    m_buffer = MakeRefCounted<SharedBuffer>(data, bufferSize.height * bytesPerRow);
}

std::unique_ptr<ImageBufferData> ImageBufferData::getData(AlphaPremultiplication outputFormat, const IntRect &rect, const IntSize &size) const {
    std::unique_ptr<ImageBufferData> result(new ImageBufferData(rect.size, static_cast<size_t>(rect.size.width * 4)));
    uint8_t *resultData = static_cast<uint8_t *>(result->data());
    if (!resultData)
        return nullptr;

    int endx = rect.getMaxX();
    int endy = rect.getMaxY();

    int originx = rect.x();
    int destx = 0;
    int destw = rect.width();
    if (originx < 0) {
        destw += originx;
        destx = -originx;
        originx = 0;
    }
    destw = std::min<int>(destw, size.width - originx);
    if (endx > size.width)
        endx = size.width;
    int width = endx - originx;

    int originy = rect.y();
    int desty = 0;
    int desth = rect.height();
    if (originy < 0) {
        desth += originy;
        desty = -originy;
        originy = 0;
    }
    desth = std::min<int>(desth, size.height - originy);
    if (endy > size.height)
        endy = size.height;
    int height = endy - originy;

    if (width <= 0 || height <= 0)
        return result;

    unsigned destBytesPerRow = static_cast<unsigned>(4 * rect.width());
    uint8_t *destRows = resultData + desty * destBytesPerRow + destx * 4;

    unsigned srcBytesPerRow;
    uint8_t *srcRows;

    if (!m_buffer->data())
        return result;

    srcBytesPerRow = m_bytesPerRow;
    srcRows = reinterpret_cast<uint8_t *>(m_buffer->data()) + originy * srcBytesPerRow + originx * 4;

    if (outputFormat == AlphaPremultiplication::Unpremultiplied) {

        vImage_Buffer src;
        src.width = width;
        src.height = height;
        src.rowBytes = srcBytesPerRow;
        src.data = srcRows;

        vImage_Buffer dest;
        dest.width = destw;
        dest.height = desth;
        dest.rowBytes = destBytesPerRow;
        dest.data = destRows;

        unpremultiplyBufferData(src, dest);

        return result;
    } else {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; x++) {
                int basex = x * 4;
                destRows[basex] = srcRows[basex + 2];
                destRows[basex + 1] = srcRows[basex + 1];
                destRows[basex + 2] = srcRows[basex];
                destRows[basex + 3] = srcRows[basex + 3];
            }
            srcRows += srcBytesPerRow;
            destRows += destBytesPerRow;
        }
    }

    return result;
}

void ImageBufferData::putData(const void *sourceData, AlphaPremultiplication sourceFormat, const IntSize &sourceSize, const IntRect &sourceRect, const IntPoint &destPoint) {


    int originx = sourceRect.x();
    int destx = destPoint.x + sourceRect.x();

    int endx = destPoint.x + sourceRect.getMaxX();

    int width = sourceRect.width();
    int destw = endx - destx;

    int originy = sourceRect.y();
    int desty = destPoint.y + sourceRect.y();

    int endy = destPoint.y + sourceRect.getMaxY();

    int height = sourceRect.height();
    int desth = endy - desty;

    if (width <= 0 || height <= 0)
        return;

    unsigned srcBytesPerRow = static_cast<unsigned int>(4 * sourceSize.width);
    const uint8_t *srcRows = reinterpret_cast<const uint8_t *>(sourceData) + (originy * srcBytesPerRow + originx * 4);
    unsigned destBytesPerRow;
    uint8_t *destRows;

    if (!m_buffer->data())
        return;

    destBytesPerRow = m_bytesPerRow;
    destRows = reinterpret_cast<uint8_t *>(m_buffer->data()) + (desty * destBytesPerRow + destx * 4);

    if (sourceFormat == AlphaPremultiplication::Unpremultiplied) {

        vImage_Buffer src;
        src.width = width;
        src.height = height;
        src.rowBytes = srcBytesPerRow;
        src.data = const_cast<uint8_t *>(srcRows);

        vImage_Buffer dest;
        dest.width = destw;
        dest.height = desth;
        dest.rowBytes = destBytesPerRow;
        dest.data = destRows;

        premultiplyBufferData(src, dest);

        return;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; x++) {
            int basex = x * 4;
            uint8_t alpha = srcRows[basex + 3];
            destRows[basex] = srcRows[basex + 2];
            destRows[basex + 1] = srcRows[basex + 1];
            destRows[basex + 2] = srcRows[basex];
            destRows[basex + 3] = alpha;
        }
        destRows += destBytesPerRow;
        srcRows += srcBytesPerRow;
    }
}

}