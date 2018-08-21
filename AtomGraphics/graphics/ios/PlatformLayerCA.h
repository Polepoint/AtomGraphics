//
// Created by neo on 2018/7/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYERCA_H
#define ATOMGRAPHICS_PLATFORMLAYERCA_H

#import "PlatformLayer.h"
#import <QuartzCore/CALayer.h>

namespace AtomGraphics {

    class PlatformLayerCA : public PlatformLayer {

    public:

        PlatformLayerCA(CALayer *caLayer) : m_layer(caLayer) {

        }

        CALayer *caLayer() {
            return m_layer;
        }

    private:
        CALayer *m_layer;
    };
}


#endif //ATOMGRAPHICS_PLATFORMLAYERCA_H
