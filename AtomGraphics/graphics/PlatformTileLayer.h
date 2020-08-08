//
// Created by neo on 2018/11/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_PLATFORMTILELAYER_H
#define ATOMGRAPHICS_PLATFORMTILELAYER_H

#include "PlatformLayer.h"
#include "TileController.h"

namespace AtomGraphics {

class PlatformTileLayer final : public PlatformLayer {

public:

    PlatformTileLayer();

    ~PlatformTileLayer() override = default;

    void setNeedsDisplay() override;

    void setNeedsDisplayInRect(const FloatRect &rect) override;

    void setNeedsToRevalidateTiles();

    void updateCoverage() override;

    void updateTiles() override;

    void updateContentScale(float scale) override;

    void updateBounds() override;

private:

    TileController m_tileController;

    friend TileController;
};

}


#endif //ATOMGRAPHICS_PLATFORMTILELAYER_H
