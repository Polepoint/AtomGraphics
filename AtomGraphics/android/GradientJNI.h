//
// Created by neo on 2019-09-28.
//

#ifndef ATOMGRAPHICS_GRADIENTJNI_H
#define ATOMGRAPHICS_GRADIENTJNI_H

#include "graphics/Gradient.h"

namespace AtomGraphics {

class GradientJNI {

public:

    GradientJNI(std::shared_ptr<Gradient> gradient) : m_gradient(gradient) {}

    std::shared_ptr<Gradient> getGradient() const {
        return m_gradient;
    }

private:

    std::shared_ptr<Gradient> m_gradient;

};

}

#endif //ATOMGRAPHICS_GRADIENTJNI_H
