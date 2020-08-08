//
// Created by neo on 2018/12/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "Image.h"


namespace AtomGraphics {

size_t Image::width() const {
    return m_width;
}

size_t Image::height() const {
    return m_height;
}

IntSize Image::size() const {
    return IntSize(m_width, m_height);
}

}