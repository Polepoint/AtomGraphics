//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_CANVASTYPES_H
#define ATOMGRAPHICS_CANVASTYPES_H

#include <string>
#include "graphics/GraphicsTypes.h"

namespace AtomGraphics {

bool parseCompositeAndBlendOperator(const std::string &, CompositeOperator &, BlendMode &);

bool parseLineJoin(const std::string &, LineJoin &);

bool parseLineCap(const std::string &, LineCap &);

bool parseFillRule(const char *, WindRule &);

bool parseTextAlign(const std::string &, TextAlign &);

bool parseTextBaseline(const std::string &, TextBaseline &);

}


#endif //ATOMGRAPHICS_CANVASTYPES_H
