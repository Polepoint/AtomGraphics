//
// Created by neo on 2019-09-28.
//

#ifndef ATOMGRAPHICS_PATTERNJNI_H
#define ATOMGRAPHICS_PATTERNJNI_H

#include "graphics/Pattern.h"

namespace AtomGraphics {

class PatternJNI {

public:

    PatternJNI(std::shared_ptr<Pattern> pattern) : m_pattern(std::move(pattern)) {}

    std::shared_ptr<Pattern> getPattern() const {
        return m_pattern;
    }

private:

    std::shared_ptr<Pattern> m_pattern;

};

}

#endif //ATOMGRAPHICS_PATTERNJNI_H
