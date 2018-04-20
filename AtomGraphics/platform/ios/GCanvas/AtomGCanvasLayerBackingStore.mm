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
        _contentDirty &= isDirtyCommand(command);
        if (_contentDirty && isFlushCommand(command)) {
            _contentDirty = false;
            return true;
        } else {
            return false;
        }
    }

    bool AtomGCanvasLayerBackingStore::isDirtyCommand(const std::string &command) {
        char shortCommand = command[0];
        return shortCommand != 'x' && shortCommand != 'L';
    }

    bool AtomGCanvasLayerBackingStore::isFlushCommand(const std::string &command) {
        char shortCommand = command[0];
        return shortCommand == 'x' || shortCommand == 'L' || shortCommand == 's';
    }

    AtomGraphicsGCanvasView *AtomGCanvasLayerBackingStore::getGCanvasView() const {
        return _gCanvasView;
    }
}