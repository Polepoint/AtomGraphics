//
// Created by neo on 2018/4/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "Gradient.h"


namespace AtomGraphics {

Gradient::~Gradient() {
    platformDestroy();
}

void Gradient::addColorStop(float offset, const Color4F &color) {
    addColorStop(ColorStop(offset, color));
}

void Gradient::addColorStop(const Gradient::ColorStop &stop) {
    m_stops.push_back(stop);
    
    m_stopsSorted = false;
    
    platformDestroy();
}

static inline bool compareStops(const Gradient::ColorStop &a, const Gradient::ColorStop &b) {
    return a.offset < b.offset;
}


void Gradient::sortStopsIfNecessary() {
    if (m_stopsSorted)
        return;
    
    m_stopsSorted = true;
    
    if (m_stops.empty())
        return;
    
    std::stable_sort(m_stops.begin(), m_stops.end(), compareStops);
}

bool Gradient::isZeroSize() const {
    if (m_isLinear) {
        return m_point0.x == m_point1.x && m_point0.y == m_point1.y;
    } else {
        return m_point0.x == m_point1.x && m_point0.y == m_point1.y && m_startRadius == m_endRadius;
    }
}


}