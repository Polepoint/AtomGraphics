//
// Created by neo on 2018/4/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "AtomGCanvasNode.h"
#import "AtomGCanvasContext2d.h"

enum {
    ContentType2D,
    ContentTypeWebGL,
};

namespace AtomGraphics {

    CanvasContext2d *GCanvasNode::getContext2d() {
        setContextType(ContentType2D);
        return _gcanvasContext2d;
    }

}
