//
// Created by neo on 2018/11/15.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "math/FloatRect.h"

namespace AtomGraphics {


FloatRect::FloatRect(const CGRect &cgRect) {
    setRect(cgRect.origin.x, cgRect.origin.y, cgRect.size.width, cgRect.size.height);
}

FloatRect::operator CGRect() const {
    return CGRectMake(origin.x, origin.y, size.width, size.height);
}

}
