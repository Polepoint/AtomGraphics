//
// Created by neo on 2018/3/30.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMPATH_H
#define ATOMGRAPHICS_ATOMPATH_H


#import "AtomGraphicsContext.h"

namespace AtomGraphics {

    class Path {

    public:
        PlatformPath platformPath() {
            return _platformPath;
        }

    private:
        PlatformPath _platformPath;
    };
}


#endif //ATOMGRAPHICS_ATOMPATH_H
