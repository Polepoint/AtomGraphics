//
// Created by neo on 2018/12/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "graphics/Image.h"


namespace AtomGraphics {

Image::Image(RetainPtr<CGImageRef> image) : m_image(std::move(image)) {
    if (m_image) {
        m_width = CGImageGetWidth(m_image.get());
        m_height = CGImageGetHeight(m_image.get());
    }
}

Image::~Image() {}

CGImageRef Image::getCGImage() const {
    return m_image.get();
}

bool Image::isOpaque() const {
    return true;
}

}