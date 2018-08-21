//
// Created by neo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "ImageBufferCG.h"
#import <cstring>
#import <cstdlib>
#import "GraphicsContextCG.h"

static void releaseImageData(void *, const void *data, size_t) {
//    delete data;
}


namespace AtomGraphics {

    ImageBufferCG::ImageBufferCG(size_t width, size_t height, size_t bytesPerRow) {
        m_backingStoreWidth = width;
        m_backingStoreHeight = height;
        m_bytesPerRow = bytesPerRow;
        m_data = malloc(m_bytesPerRow * m_backingStoreHeight);
        memset(m_data, 0, m_bytesPerRow * m_backingStoreHeight);
        m_DataProvider = CGDataProviderCreateWithData(0, m_data, height * m_bytesPerRow, releaseImageData);
    }

    GraphicsContext *ImageBufferCG::createGraphicsContext() {
//        CGBitmapContextCreateWithData(m_data, m_backingStoreWidth, m_backingStoreHeight, 8, m_bytesPerRow, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Host, releaseBitmapContextData, this);
        CGContextRef bitmapContext = CGBitmapContextCreate(m_data, m_backingStoreWidth, m_backingStoreHeight, 8, m_bytesPerRow, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
        CGContextTranslateCTM(bitmapContext, 0, m_backingStoreHeight);
        CGContextScaleCTM(bitmapContext, 1, -1);
        GraphicsContextCG *contextCG = new GraphicsContextCG(bitmapContext);
        return contextCG;
    }

    void ImageBufferCG::releaseBitmapContextData(void *typelessBitmap, void *typelessData) {

    }

}