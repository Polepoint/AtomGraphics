//
// Created by neo on 2018/11/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TILECONTROLLER_H
#define ATOMGRAPHICS_TILECONTROLLER_H


#include "math/AtomGeometry.h"
#include "TileGrid.h"
#include "TileGridLayer.h"
#include "TileLayerPool.h"
#include "base/Device.h"

namespace AtomGraphics {

const int kDefaultTileSize = 512;

class PlatformTileLayer;

class TileGrid;

class TileController {

public:
    
    TileController(PlatformTileLayer *layer);
    
    IntSize tileSize();
    
    std::unique_ptr<TileGridLayer> createTileLayer(const IntRect &tileRect);
    
    void moveLayerToLayerPool(std::unique_ptr<TileGridLayer> layer);
    
    void setNeedsDisplay();
    
    void setNeedsDisplayInRect(const FloatRect &rect);
    
    float contentsScale() const;
    
    void setContentsScale(float contentScale) const;
    
    float getDeviceScaleFactor() {
        return Device::DeviceScaleFactor();
    }
    
    const FloatRect &getCoverageRect() const {
        return m_coverageRect;
    }
    
    void setCoverageRect(const FloatRect &rect);
    
    const FloatRect &getBounds() const {
        return m_bounds;
    }
    
    void setBounds(const FloatRect &bounds) {
        m_bounds = bounds;
    }
    
    IntRect boundsAtLastRevalidate();
    
    void revalidateTiles();
    
    void didRevalidateTiles();

private:
    
    FloatRect m_coverageRect;
    FloatRect m_bounds;
    IntRect m_boundsAtLastRevalidate;
    TileLayerPool m_layerPool;
    PlatformTileLayer *m_owner;
    
    std::unique_ptr<TileGrid> m_tileGrid;
    
    void setNeedsRevalidateTiles();
    
    TileGrid &tileGrid() const {
        return *m_tileGrid;
    }
    
    PlatformTileLayer *owningGraphicsLayer() const {
        return m_owner;
    }
};

}


#endif //ATOMGRAPHICS_TILECONTROLLER_H
