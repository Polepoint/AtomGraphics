//
// Created by neo on 2019/5/25.
//

#ifndef ATOMGRAPHICS_GLINTERFACE_H
#define ATOMGRAPHICS_GLINTERFACE_H

#include <skia/gpu/gl/GrGLInterface.h>

namespace AtomGraphics {

class GLInterface {

public:
    static sk_sp<const GrGLInterface> MakeDebugInterface(bool useGLES2);

};

}


#endif //ATOMGRAPHICS_GLINTERFACE_H
