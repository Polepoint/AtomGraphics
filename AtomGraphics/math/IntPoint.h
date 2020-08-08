//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_INTPOINT_H
#define ATOMGRAPHICS_INTPOINT_H

#include "IntSize.h"

namespace AtomGraphics {

class IntPoint {

public:
    int x;
    int y;
    
    IntPoint();
    
    IntPoint(int x, int y);
    
    IntPoint(const IntPoint &other);
    
    IntPoint(const IntSize &size);
    
    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
    
    IntPoint &operator=(const IntPoint &other);
    
};

inline IntPoint &operator+=(IntPoint &a, const IntSize &b) {
    a.move(b.width, b.height);
    return a;
}

}


#endif //ATOMGRAPHICS_INTPOINT_H
