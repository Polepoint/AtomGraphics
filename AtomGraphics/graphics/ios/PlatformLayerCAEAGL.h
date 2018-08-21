//
// Created by neo on 2018/7/11.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYERCAEAGL_H
#define ATOMGRAPHICS_PLATFORMLAYERCAEAGL_H

#import "PlatformLayer.h"
#import "AtomGCanvasEAGLLayer.h"

namespace AtomGraphics {

    class PlatformLayerCAEAGL : public PlatformLayer {

    public:
        PlatformLayerCAEAGL(AtomGCanvasEAGLLayer *caLayer) : m_layer(caLayer) {

        }

        AtomGCanvasEAGLLayer *glLayer() {
            return m_layer;
        }

    private:
        AtomGCanvasEAGLLayer *m_layer;
    };
}


#endif //ATOMGRAPHICS_PLATFORMLAYERCAEAGL_H
