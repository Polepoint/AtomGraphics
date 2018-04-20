//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <string>
#import "AtomLayerBackingStore.h"

@class AtomGraphicsGCanvasView;

namespace AtomGraphics {

    class AtomGCanvasLayerBackingStore : public AtomLayerBackingStore {

    public:

        AtomGCanvasLayerBackingStore(AtomGraphicsGCanvasView *gCanvasView);

        void applyBackingStoreToLayer(CALayer *layer);

        void flush();

        bool addCommand(const std::string &command);

        AtomGraphicsGCanvasView *getGCanvasView() const;

    private:
        std::string _renderCommandLine;
        AtomGraphicsGCanvasView *_gCanvasView;

        bool _contentDirty;

        bool isDirtyCommand(const std::string &command);

        bool isFlushCommand(const std::string &command);
    };

}