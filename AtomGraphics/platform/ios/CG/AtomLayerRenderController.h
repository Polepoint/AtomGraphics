//
// Created by neo on 2018/4/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMLAYERRENDERCONTROLLER_H
#define ATOMLAYERRENDERCONTROLLER_H

#import <QuartzCore/CALayer.h>

namespace AtomGraphics {

    class AtomLayerRenderController {

    public:

        AtomLayerRenderController();

        void updateLayers(NSArray *layerIDs);

    private:

        void dumpChangedLayer(CALayer *layer);

        NSArray *_changedLayerProperties;

        NSMutableDictionary *_layers;

    };
}


#endif //ATOMLAYERRENDERCONTROLLER_H
