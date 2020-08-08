//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_COLORCG_H
#define ATOMGRAPHICS_COLORCG_H

#import <CoreGraphics/CGColor.h>
#import "ColorCG.h"
#import "AtomTypes.h"

namespace AtomGraphics {

CGColorSpaceRef sRGBColorSpaceRef();

CGColorRef cachedCGColor(const Color4F &color);

}


#endif //ATOMGRAPHICS_COLORCG_H
