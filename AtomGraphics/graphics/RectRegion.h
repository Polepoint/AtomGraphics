//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_REGION_H
#define ATOMGRAPHICS_REGION_H

#include <vector>
#include "math/AtomGeometry.h"

namespace AtomGraphics {

class RectRegion {

public:
    
    RectRegion();
    
    RectRegion(const IntRect &);
    
    IntRect bounds() const {return m_bounds;}
    
    bool isEmpty() const {return m_bounds.isEmpty();}
    
    std::vector<IntRect> rects() const;
    
    void unite(const RectRegion &);
    
    bool contains(const RectRegion &region) const;
    
    unsigned totalArea() const;

private:
    
    std::vector<IntRect> m_rects;
    IntRect m_bounds;
    
};

}


#endif //ATOMGRAPHICS_REGION_H
