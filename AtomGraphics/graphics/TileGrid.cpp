//
// Created by neo on 2018/11/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <sys/param.h>
#include "TileGrid.h"

namespace AtomGraphics {

TileGrid::TileGrid(TileController &controller) : m_controller(controller) {

}


void TileGrid::setNeedsDisplay() {
    for (auto &entry : m_tiles) {
        TileInfo &tileInfo = entry.second;
        IntRect tileRect = rectForTileIndex(entry.first);

        if (tileRect.intersectsRect(m_tileCoverageRect))
            tileInfo.layer->setNeedsDisplay();
        else
            tileInfo.hasStaleContent = true;
    }
}

void TileGrid::setNeedsDisplayInRect(const FloatRect &rect) {
    if (!m_tiles.size())
        return;

    FloatRect scaledRect(rect);
    scaledRect.scale(m_scale);
    IntRect repaintRectInTileCoords(enclosingIntRect(scaledRect));

    IntSize tileSize = m_tileSize;

    // For small invalidations, lookup the covered tiles.
    if (repaintRectInTileCoords.height() < 2 * tileSize.height &&
            repaintRectInTileCoords.width() < 2 * tileSize.width) {
        GridRange range;
        if (tileRangeInViewRect(repaintRectInTileCoords, &range)) {
            TileIndex topLeft(range.row, range.column);
            TileIndex bottomRight(range.row + range.rowSpan - 1, range.column + range.columnSpan - 1);
            for (int y = topLeft.row; y <= bottomRight.row; ++y) {
                for (int x = topLeft.column; x <= bottomRight.column; ++x) {
                    TileIndex tileIndex(x, y);

                    auto it = m_tiles.find(tileIndex);
                    if (it != m_tiles.end())
                        setTileNeedsDisplayInRect(tileIndex, it->second, repaintRectInTileCoords, m_tileCoverageRect);
                }
            }
        }
        return;
    }

    for (auto it = m_tiles.begin(), end = m_tiles.end(); it != end; ++it)
        setTileNeedsDisplayInRect(it->first, it->second, repaintRectInTileCoords, m_tileCoverageRect);
}

void TileGrid::setTileNeedsDisplayInRect(const TileIndex &tileIndex, TileInfo &tileInfo,
                                         const IntRect &repaintRectInTileCoords,
                                         const IntRect &coverageRectInTileCoords) {
    TileGridLayer *tileLayer = tileInfo.layer.get();

    IntRect tileRect = rectForTileIndex(tileIndex);
    FloatRect tileRepaintRect = tileRect;
    tileRepaintRect.intersect(repaintRectInTileCoords);
    if (tileRepaintRect.isEmpty())
        return;

    tileRepaintRect.origin.x -= tileRect.origin.x;
    tileRepaintRect.origin.y -= tileRect.origin.y;

    if (tileRect.intersectsRect(coverageRectInTileCoords) && tileLayer->superlayer()) {
        tileLayer->setNeedsDisplayInRect(tileRepaintRect);
    } else
        tileInfo.hasStaleContent = true;
}

IntRect TileGrid::ensureTilesForRect(const FloatRect &rect) {

    FloatRect scaledRect(rect);
    scaledRect.scale(m_scale);
    GridRange rectRange;
    if (!tileRangeInViewRect(rect, &rectRange)) {
        return IntRect();
    };

    IntRect coverageRect;

    for (int y = rectRange.row; y < rectRange.row + rectRange.rowSpan; y++) {
        for (int x = rectRange.column; x < rectRange.column + rectRange.columnSpan; x++) {
            TileIndex tileIndex(x, y);
            IntRect tileRect = rectForTileIndex(tileIndex);
            TileInfo &tileInfo = m_tiles.emplace(tileIndex, TileInfo()).first->second;
            tileInfo.visible = true;

            coverageRect.merge(tileRect);

            if (!tileInfo.layer) {
                tileInfo.layer = m_controller.createTileLayer(tileRect);
            } else {
                // We already have a layer for this tile. Ensure that its size is correct.
                bool shouldChangeTileLayerFrame = !tileInfo.layer->getBounds().size.equals(FloatSize(tileRect.size))
                        || !tileInfo.layer->getPosition().equals(FloatPoint(tileRect.origin));

                if (shouldChangeTileLayerFrame) {
                    tileInfo.layer->setBounds(FloatRect(0, 0, tileRect.size.width, tileRect.size.height));
                    tileInfo.layer->setPosition(FloatPoint(tileRect.origin.x, tileRect.origin.y));
                    tileInfo.layer->setNeedsDisplay();
                }
            }
        }
    }

    return coverageRect;
}

void TileGrid::revalidateTiles() {

    FloatRect coverageRect = m_controller.getCoverageRect();
    IntRect bounds = m_controller.getBounds();

    IntSize tileSize = m_controller.tileSize();

    bool hasInvisibleTile = false;

    if (!m_tileSize.equals(tileSize)) {
        //Remove tiles
        m_tileSize = tileSize;
    }

    for (auto &entry: m_tiles) {
        TileInfo &tileInfo = entry.second;

        TileGridLayer *tileLayer = tileInfo.layer.get();
        IntRect tileRect = rectForTileIndex(entry.first);
        if (!tileRect.isEmpty() && tileRect.intersectsRect(coverageRect)) {
            tileInfo.visible = true;
            if (tileInfo.hasStaleContent) {
                // FIXME: store a dirty region per layer?
                tileLayer->setNeedsDisplay();
                tileInfo.hasStaleContent = false;
            }
        } else {
            // Add to the currentCohort if not already in one.
            if (tileInfo.visible) {
                tileInfo.visible = false;
                hasInvisibleTile = true;
//                tileLayer->removeFromSuperlayer();
            }
        }
    }

    if (hasInvisibleTile) {
        removeInvisibleTiles();
    }

//    if (validationPolicy & UnparentAllTiles) {
//        for (auto &tile : m_tiles.values())
//            tile.layer->removeFromSuperlayer();
//    }

    auto boundsAtLastRevalidate = m_controller.boundsAtLastRevalidate();
    if (!boundsAtLastRevalidate.equals(bounds)) {
        // If there are margin tiles and the bounds have grown taller or wider, then the tiles that used to
        // be bottom or right margin tiles need to be invalidated.

        FloatRect scaledBounds(bounds);
        scaledBounds.scale(m_scale);
        IntRect boundsInTileCoords(scaledBounds);

        GridRange range;
        if (tileRangeInViewRect(boundsInTileCoords, &range)) {
            std::vector<TileIndex> tilesToRemove;
            for (auto &entry : m_tiles) {
                auto index = entry.first;
                if (index.row < range.row || index.row > range.row + range.rowSpan || index.column < range.column ||
                        index.column > range.column + range.columnSpan)
                    tilesToRemove.push_back(index);
            }
            removeTiles(tilesToRemove);
        }
    }

    // Ensure primary tile coverage tiles.
    m_tileCoverageRect = ensureTilesForRect(bounds);

    m_controller.didRevalidateTiles();
}


void TileGrid::removeInvisibleTiles() {
    std::vector<TileIndex> tilesToRemove;

    for (auto &entry : m_tiles) {
        const TileInfo &tileInfo = entry.second;
        if (tileInfo.visible)
            continue;
        tilesToRemove.push_back(entry.first);
    }

    removeTiles(tilesToRemove);
}


bool TileGrid::tileRangeInViewRect(const FloatRect &viewRect, GridRange *rangeResult) {
    IntRect clampedRect = m_controller.getBounds();
    clampedRect.scale(m_scale);
    clampedRect.intersect(viewRect);

    if (clampedRect.isEmpty())
        return false;

    auto tileSize = m_tileSize;
    if (clampedRect.origin.x >= 0)
        rangeResult->column = clampedRect.origin.x / tileSize.width;
    else
        rangeResult->column = static_cast<int>(floorf((float) clampedRect.origin.x / tileSize.width));

    if (clampedRect.origin.y >= 0)
        rangeResult->row = clampedRect.origin.y / tileSize.height;
    else
        rangeResult->row = static_cast<int>(floorf((float) clampedRect.origin.y / tileSize.height));

    int bottomXRatio = static_cast<int>(ceil((float) clampedRect.getMaxX() / tileSize.width));
    rangeResult->columnSpan = std::max(bottomXRatio, 0) - rangeResult->column;

    int bottomYRatio = static_cast<int>(ceil((float) clampedRect.getMaxY() / tileSize.height));
    rangeResult->rowSpan = std::max(bottomYRatio, 0) - rangeResult->row;

    return true;
}


IntRect TileGrid::rectForTileIndex(const TileIndex &tileIndex) const {
    IntSize tileSize = m_tileSize;
    IntRect rect(tileIndex.row * tileSize.width, tileIndex.column * tileSize.height, tileSize.width, tileSize.height);
    IntRect scaledBounds(m_controller.getBounds());
    scaledBounds.scale(m_scale);
    rect.intersect(scaledBounds);
    return rect;
}

void TileGrid::removeTiles(const std::vector<TileIndex> &tilesToRemove) {
    for (size_t i = 0; i < tilesToRemove.size(); ++i) {
        auto it = m_tiles.find(tilesToRemove[i]);
        if (it != m_tiles.end()) {
            TileInfo &tileInfo = it->second;
            tileInfo.layer->removeFromSuperlayer();
            m_controller.moveLayerToLayerPool(std::move(tileInfo.layer));
            m_tiles.erase(it);
        }
    }
}

}
