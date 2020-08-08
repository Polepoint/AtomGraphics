//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "FastMalloc.h"
#include <cstdlib>

namespace AtomGraphics {

namespace base {

void *fastMalloc(size_t n) {
    void *result = malloc(n);
//    if (!result)
//        CRASH();
    
    return result;
}

void fastFree(void *p) {
    free(p);
}

} // namespace base

} // namespace GraphicsEnvironment


