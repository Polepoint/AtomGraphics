//
// Created by neo on 2018/11/30.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_COREGRAPHICSAPIEXTENSION_H
#define ATOMGRAPHICS_COREGRAPHICSAPIEXTENSION_H

#include <CoreGraphics/CoreGraphics.h>

namespace AtomGraphics {


void ContextSetCTM(CGContextRef, CGAffineTransform);

void ContextDrawPath(CGContextRef, CGPathDrawingMode, CGPathRef, const CGRect *boundingBox);

CGPatternRef PatternCreateWithImage(CGImageRef, CGAffineTransform, CGPatternTiling);

}

#endif //ATOMGRAPHICS_COREGRAPHICSAPIEXTENSION_H
