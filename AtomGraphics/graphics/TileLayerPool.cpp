//
// Created by neo on 2018/11/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "TileLayerPool.h"
#include "thread/GraphicsThread.h"

using namespace std;

namespace AtomGraphics {

static const TimeInterval capacityDecayTime{5};

TileLayerPool::TileLayerPool() :
        m_totalBytes(0),
        m_maxBytesForPool(48 * 1024 * 1024),
        m_pruneTimer(GraphicsThread::GraphicsThreadTaskRunner(),
                     *this,
                     &TileLayerPool::pruneTimerFired) {

}

void TileLayerPool::addLayer(unique_ptr<TileGridLayer> layer) {
    IntSize layerSize(expandedIntSize(layer->getBounds().size));
    if (!canReuseLayerWithSize(layerSize))
        return;

    listOfLayersWithSize(layerSize).m_queue.push_front(std::move(layer));
    m_totalBytes += backingStoreBytesForSize(layerSize);
    m_lastAddTime = Clock::now();
    schedulePrune();
}

std::unique_ptr<TileGridLayer> TileLayerPool::takeLayerWithSize(const IntSize &size) {
    if (!canReuseLayerWithSize(size))
        return nullptr;
    LayerList &reuseList = listOfLayersWithSize(size, MarkAsUsed);
    if (!reuseList.m_queue.size())
        return nullptr;
    m_totalBytes -= backingStoreBytesForSize(size);
    std::unique_ptr<TileGridLayer> layer = std::move(reuseList.m_queue.front());
    reuseList.m_queue.pop_front();
    return layer;
}

unsigned TileLayerPool::backingStoreBytesForSize(const IntSize &size) {
    return static_cast<unsigned>(size.width * size.height * 4);
}

TileLayerPool::LayerList &TileLayerPool::listOfLayersWithSize(const IntSize &size, AccessType accessType) {
    map<IntSize, LayerList>::iterator it = m_reuseLists.find(size);
    if (it == m_reuseLists.end()) {
        it = m_reuseLists.emplace(size, LayerList()).first;
        m_sizesInPruneOrder.push_back(size);
    } else if (accessType == MarkAsUsed) {
        //move the size to the end of m_sizesInPruneOrder
        auto iterator = std::find(m_sizesInPruneOrder.begin(), m_sizesInPruneOrder.end(), size);
        m_sizesInPruneOrder.erase(iterator);
        m_sizesInPruneOrder.push_back(size);
    }

    return it->second;
}

unsigned TileLayerPool::decayedCapacity() const {
    // Decay to one quarter over capacityDecayTime
    TimeInterval timeSinceLastAdd = Clock::now() - m_lastAddTime;
    if (timeSinceLastAdd > capacityDecayTime)
        return m_maxBytesForPool / 4;
    float decayProgress = float(timeSinceLastAdd / capacityDecayTime);
    return static_cast<unsigned>(m_maxBytesForPool / 4 + m_maxBytesForPool * 3 / 4 * (1 - decayProgress));
}

void TileLayerPool::schedulePrune() {
    if (m_pruneTimer.isActive())
        return;
    m_pruneTimer.startOneShot(1);
}


void TileLayerPool::pruneTimerFired() {
    unsigned shrinkTo = decayedCapacity();
    while (m_totalBytes > shrinkTo) {
        IntSize sizeToDrop = m_sizesInPruneOrder.front();
        std::deque<std::unique_ptr<TileGridLayer>> &oldestReuseList = m_reuseLists.find(sizeToDrop)->second.queue();
        if (oldestReuseList.empty()) {
            m_reuseLists.erase(sizeToDrop);
            m_sizesInPruneOrder.erase(m_sizesInPruneOrder.begin());
            continue;
        }

        m_totalBytes -= backingStoreBytesForSize(sizeToDrop);
        // The last element in the list is the oldest, hence most likely not to
        // still have a backing store.
        oldestReuseList.pop_back();
    }
    if (Clock::now() - m_lastAddTime <= capacityDecayTime)
        schedulePrune();
}

}