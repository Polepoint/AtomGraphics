//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGCanvasLayerBackingStore.h"
#import "AtomGraphicsGCanvasView.h"
#import "AtomGraphicsGCanvasModule.h"


namespace AtomGraphics {

    AtomGCanvasLayerBackingStore::AtomGCanvasLayerBackingStore(AtomGraphicsGCanvasView *gCanvasView)
            : _gCanvasView(gCanvasView) {

    }

    void AtomGCanvasLayerBackingStore::applyBackingStoreToLayer(CALayer *layer) {
        [[AtomGraphicsGCanvasModule sharedModule].gcanvasModule render:[NSString stringWithCString:_renderCommandLine.c_str()] componentId:_gCanvasView.componentId];
    }

    void AtomGCanvasLayerBackingStore::flush() {

    }

    bool AtomGCanvasLayerBackingStore::addCommand(const std::string &command) {
        _renderCommandLine += command + ";";
        if (_contentChanged && (command[0] == 'x' || command[0] == 'L')) {
            _contentChanged = false;
            return true;
        } else {
            _contentChanged = true;
            return false;
        }
    }

    AtomGraphicsGCanvasView *AtomGCanvasLayerBackingStore::getGCanvasView() const {
        return _gCanvasView;
    }
}