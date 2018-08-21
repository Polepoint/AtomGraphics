//
// Created by woo on 2018/4/20.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AtomCanvasImage.h"

AtomGraphics::AtomCanvasImage::AtomCanvasImage() {
    mId = autoIncreasedId++;
    mWidth = 0;
    mHeight = 0;
}

float AtomGraphics::AtomCanvasImage::getWidth() const {
    return mWidth;
}

float AtomGraphics::AtomCanvasImage::getHeight() const {
    return mHeight;
}

const std::string &AtomGraphics::AtomCanvasImage::getSrc() const {
    return mSrc;
}

void AtomGraphics::AtomCanvasImage::setWidth(float width) {
    AtomCanvasImage::mWidth = width;
}

void AtomGraphics::AtomCanvasImage::setHeight(float height) {
    AtomCanvasImage::mHeight = height;
}

void AtomGraphics::AtomCanvasImage::setSrc(const std::string &src) {
    AtomCanvasImage::mSrc = src;
}

int AtomGraphics::AtomCanvasImage::getId() const {
    return mId;
}
