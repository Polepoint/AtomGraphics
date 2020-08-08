//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "RectRegion.h"

namespace AtomGraphics {

RectRegion::RectRegion() {}

RectRegion::RectRegion(const IntRect &rect) : m_bounds(rect) {
    m_rects.push_back(rect);
}

std::vector<IntRect> RectRegion::rects() const {
    std::vector<IntRect> rects(m_rects);
    return rects;
}

void RectRegion::unite(const RectRegion &region) {
    if (region.isEmpty())
        return;

    if (isEmpty()) {
        m_bounds = region.m_bounds;
        m_rects = region.m_rects;
        return;
    }

    if (m_bounds.containsRect(region.m_bounds))
        return;
    if (region.m_bounds.containsRect(m_bounds)) {
        m_bounds = region.m_bounds;
        m_rects = region.m_rects;
        return;
    }

    if (contains(region)) {
        return;
    }

    std::vector<IntRect> unitedShapeRects(m_rects);

    auto iteratorB = region.m_rects.begin();
    while (iteratorB != region.m_rects.end()) {
        bool contains = false;
        auto iteratorA = m_rects.begin();
        while (iteratorA != m_rects.end()) {
            if (iteratorB->containsRect(*iteratorA)) {
                contains = true;
                unitedShapeRects.erase(iteratorA);
                unitedShapeRects.push_back(*iteratorB);
            }
            iteratorA++;
        }
        if (!contains) {
            unitedShapeRects.push_back(*iteratorB);
        }
        iteratorB++;
    }

    m_rects = unitedShapeRects;
    m_bounds.unite(region.m_bounds);
}

bool RectRegion::contains(const RectRegion &region) const {
    return m_bounds.containsRect(region.m_bounds);
}

unsigned RectRegion::totalArea() const {
    std::vector<IntRect> rects = this->rects();
    size_t size = rects.size();
    unsigned totalArea = 0;

    for (size_t i = 0; i < size; ++i) {
        IntRect rect = rects[i];
        totalArea += (rect.size.width * rect.size.height);
    }

    return totalArea;
}


}