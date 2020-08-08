//
// Created by neo on 2019/3/13.
//

#ifndef ATOMGRAPHICS_SCREENDISPLAY_H
#define ATOMGRAPHICS_SCREENDISPLAY_H

#include <cstdint>
#include "math/IntRect.h"

namespace AtomGraphics {

class ScreenDisplay {

public:
    ScreenDisplay() {}

    ScreenDisplay(int64_t id) : m_id(id) {};

    ScreenDisplay(int64_t id, const IntRect &bounds);

    ScreenDisplay(const ScreenDisplay &other) = default;

    int64_t id() const {
        return m_id;
    }

    /**
     * set bounds in dip
     * @param bounds
     */
    void setBounds(const IntRect &bounds_in_dip) {
        m_bounds = bounds_in_dip;
    }

    void setSizeInPixels(const IntSize &sizeInPixels) {
        m_sizeInPixels = sizeInPixels;
    }

    float getDeviceScaleFactor() const {
        return m_deviceScaleFactor;
    }

    void setDeviceScaleFactor(float deviceScaleFactor) {
        m_deviceScaleFactor = deviceScaleFactor;
    }

    void setColorDepth(int colorDepth) {
        m_colorDepth = colorDepth;
    }

    void setDepthPerComponent(int depthPerComponent) {
        m_depthPerComponent = depthPerComponent;
    }

private:
    int64_t m_id;

    IntRect m_bounds;
    // If non-empty, then should be same size as |bounds_|. Used to avoid rounding
    // errors.
    IntSize m_sizeInPixels;
    float m_deviceScaleFactor;
    int m_colorDepth;
    int m_depthPerComponent;
};

}


#endif //ATOMGRAPHICS_SCREENDISPLAY_H
