//
// Created by neo on 2018/11/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TILEGRID_H
#define ATOMGRAPHICS_TILEGRID_H

#include <map>
#include <climits>
#include "base/AtomTypes.h"
#include "PlatformLayer.h"
#include "TileController.h"

namespace AtomGraphics {

class TileGridLayer;

class TileController;

struct GridRange {
    int column{0};
    int row{0};
    int columnSpan{0};
    int rowSpan{0};
};

struct TileIndex {
    int row;
    int column;
    
    TileIndex() {
        row = 0;
        column = 0;
    }
    
    TileIndex(int x, int y) {
        row = x;
        column = y;
    }
    
    bool operator==(const TileIndex &index) const {
        return row == index.row && column == index.column;
    }
    
    bool operator<(const TileIndex &index) const {
        if (row != index.row) {
            return row < index.row;
        }
        
        if (column != index.column) {
            return column < index.column;
        }
        
        return false;
    }
};


struct TileInfo {
    //mark a off-screen tile area that should be redraw if it is moved in the visible rect in the future
    bool hasStaleContent;
    bool visible;
    std::unique_ptr<TileGridLayer> layer{nullptr};
    
    TileInfo() : hasStaleContent{false} {
    
    }
};


class TileGrid {

public:
    
    TileGrid(TileController &controller);
    
    void setNeedsDisplay();
    
    void setNeedsDisplayInRect(const FloatRect &rect);
    
    IntRect ensureTilesForRect(const FloatRect &rect);
    
    void revalidateTiles();
    
    bool tileRangeInViewRect(const FloatRect &viewRect, GridRange *rangeResult);
    
    float scale() {
        return m_scale;
    }
    
    void setScale(float scale) {
        m_scale = scale;
    }
    
    const IntRect &getBounds() const {
        return m_bounds;
    }
    
    void setBounds(const IntRect &bounds) {
        m_bounds = bounds;
    }


private:
    
    IntRect rectForTileIndex(const TileIndex &tileIndex) const;
    
    TileController &m_controller;
    
    std::map<TileIndex, TileInfo> m_tiles;
    float m_scale{1};
    IntSize m_tileSize;
    IntRect m_bounds;
    IntRect m_tileCoverageRect;
    
    void removeInvisibleTiles();
    
    void removeTiles(const std::vector<TileIndex> &tilesToRemove);
    
    void setTileNeedsDisplayInRect(const TileIndex &tileIndex, TileInfo &tileInfo, const IntRect &repaintRectInTileCoords, const IntRect &coverageRectInTileCoords);
};

}


#endif //ATOMGRAPHICS_TILEGRID_H
