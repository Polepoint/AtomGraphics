//
// Created by neo on 2018/12/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FLOATQUAD_H
#define ATOMGRAPHICS_FLOATQUAD_H

#include "FloatPoint.h"
#include "FloatRect.h"

namespace AtomGraphics {

class FloatQuad {

public:
    FloatQuad() {}
    
    FloatQuad(const FloatPoint &p1, const FloatPoint &p2, const FloatPoint &p3, const FloatPoint &p4)
            : m_p1(p1), m_p2(p2), m_p3(p3), m_p4(p4) {
    }
    
    FloatQuad(const FloatRect &inRect) : m_p1(inRect.origin),
                                         m_p2(inRect.getMaxX(), inRect.y()),
                                         m_p3(inRect.getMaxX(), inRect.getMaxY()),
                                         m_p4(inRect.x(), inRect.getMaxY()) {
    }
    
    const FloatPoint &p1() const {return m_p1;}
    
    const FloatPoint &p2() const {return m_p2;}
    
    const FloatPoint &p3() const {return m_p3;}
    
    const FloatPoint &p4() const {return m_p4;}
    
    void setP1(const FloatPoint &p) {m_p1 = p;}
    
    void setP2(const FloatPoint &p) {m_p2 = p;}
    
    void setP3(const FloatPoint &p) {m_p3 = p;}
    
    void setP4(const FloatPoint &p) {m_p4 = p;}
    
    void move(float dx, float dy) {
        m_p1.move(dx, dy);
        m_p2.move(dx, dy);
        m_p3.move(dx, dy);
        m_p4.move(dx, dy);
    }
    
    bool containsPoint(const FloatPoint&) const;
    
    bool containsQuad(const FloatQuad&) const;
    
    bool isEmpty() const {return boundingBox().isEmpty();}
    
    FloatRect boundingBox() const;

private:
    FloatPoint m_p1;
    FloatPoint m_p2;
    FloatPoint m_p3;
    FloatPoint m_p4;
};

}


#endif //ATOMGRAPHICS_FLOATQUAD_H
