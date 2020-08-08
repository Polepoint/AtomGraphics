//
// Created by neo on 2018/8/25.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TYPEDEFINE_H
#define ATOMGRAPHICS_TYPEDEFINE_H

#include <stddef.h>
#include <stdint.h>

namespace AtomGraphics {

typedef unsigned short Glyph;

/**
 * UTF16/Unicode Character Type
 */
typedef __CHAR16_TYPE__ UChar;

typedef int32_t UChar32;

constexpr size_t NotFound = static_cast<size_t>(-1);

}

#endif //ATOMGRAPHICS_TYPEDEFINE_H
