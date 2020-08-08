//
// Created by neo on 2019/3/13.
//

#include "ScreenDisplay.h"


namespace AtomGraphics {

constexpr int DEFAULT_BITS_PER_PIXEL = 24;
constexpr int DEFAULT_BITS_PER_COMPONENT = 8;

ScreenDisplay::ScreenDisplay(int64_t id, const IntRect &bounds)
        : m_id(id),
          m_bounds(bounds),
          m_deviceScaleFactor(1),
          m_colorDepth(DEFAULT_BITS_PER_PIXEL),
          m_depthPerComponent(DEFAULT_BITS_PER_COMPONENT) {

}

}