//
// Created by neo on 2018/11/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_LAYERPOOL_H
#define ATOMGRAPHICS_LAYERPOOL_H

#include <deque>
#include <map>
#include "thread/Timer.h"
#include "math/AtomGeometry.h"
#include "TileGridLayer.h"

namespace AtomGraphics {

class TileLayerPool {

public:
    
    TileLayerPool();
    
    void addLayer(std::unique_ptr<TileGridLayer> layer);
    
    std::unique_ptr<TileGridLayer> takeLayerWithSize(const IntSize &size);

private:
    
    struct LayerList {
        std::deque<std::unique_ptr<TileGridLayer>> m_queue;
        
        LayerList() {};
        
        std::deque<std::unique_ptr<TileGridLayer>> &queue() {return m_queue;}
    };
    
    typedef enum {
        LeaveUnchanged, MarkAsUsed
    } AccessType;
    
    LayerList &listOfLayersWithSize(const IntSize &size, AccessType = LeaveUnchanged);
    
    unsigned int backingStoreBytesForSize(const IntSize &size);
    
    bool canReuseLayerWithSize(const IntSize &size) const {
        return m_maxBytesForPool && !size.isEmpty();
    }
    
    unsigned decayedCapacity() const;
    
    void schedulePrune();
    
    void pruneTimerFired();
    
    
    std::map<IntSize, LayerList> m_reuseLists;
    std::vector<IntSize> m_sizesInPruneOrder;
    unsigned m_totalBytes;
    unsigned m_maxBytesForPool;
    Timer m_pruneTimer;
    TimeInterval m_lastAddTime;
    
};

}


#endif //ATOMGRAPHICS_LAYERPOOL_H
