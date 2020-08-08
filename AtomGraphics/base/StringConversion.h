//
// Created by neo on 2018/12/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_STRINGCONVERSION_H
#define ATOMGRAPHICS_STRINGCONVERSION_H

#include <cstddef>

namespace AtomGraphics {

class StringConversion {

public:

    static bool stringToNumber(const char *numberToken, size_t length, double &result);

    static char *doubleToString(double number);
};

}


#endif //ATOMGRAPHICS_STRINGCONVERSION_H
