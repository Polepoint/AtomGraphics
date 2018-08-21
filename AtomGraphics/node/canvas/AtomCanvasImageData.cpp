//
// Created by woo on 2018/4/23.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasImageData.h"

AtomGraphics::AtomCanvasImageData::AtomCanvasImageData(int mWidth, int mHeight) : mWidth(mWidth), mHeight(mHeight) {
}

int AtomGraphics::AtomCanvasImageData::getWidth() const {
    return mWidth;
}

int AtomGraphics::AtomCanvasImageData::getHeight() const {
    return mHeight;
}

int *AtomGraphics::AtomCanvasImageData::getData() const {
    return mData;
}

void AtomGraphics::AtomCanvasImageData::setData(int *mData) {
    AtomCanvasImageData::mData = mData;
}
