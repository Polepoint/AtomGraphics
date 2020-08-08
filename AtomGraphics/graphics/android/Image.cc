//
// Created by neo on 2019/1/4.
//

#include "graphics/Image.h"

namespace AtomGraphics {

Image::Image(sk_sp<SkImage> image) {
    m_skImage = image;
    if(image){
        m_width = static_cast<size_t>(image->width());
        m_height = static_cast<size_t>(image->height());
    }
}

Image::~Image() {

}

sk_sp<SkImage> Image::getSkImage() {
    return m_skImage;
}

bool Image::isOpaque() const {
    return m_skImage->isOpaque();
}

}