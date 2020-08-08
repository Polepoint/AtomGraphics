//
// Created by neo on 2018/8/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "TextMetrics.h"

namespace AtomGraphics {


float TextMetrics::getWidth() const {
    return m_width;
}

void TextMetrics::setWidth(float width) {
    m_width = width;
}

}