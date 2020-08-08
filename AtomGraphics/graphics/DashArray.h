//
// Created by neo on 2019/1/3.
//

#ifndef ATOMGRAPHICS_DASHARRAY_H
#define ATOMGRAPHICS_DASHARRAY_H

#include "platform/AtomPlatformConfig.h"
#include <vector>

namespace AtomGraphics {

#if PLATFORM(IOS)

typedef CGFloat DashArrayElement;

#else

typedef float DashArrayElement;

#endif


typedef std::vector<DashArrayElement> DashArray;

}


#endif //ATOMGRAPHICS_DASHARRAY_H
