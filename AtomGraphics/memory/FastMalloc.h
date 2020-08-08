//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FASTMALLOC_H
#define ATOMGRAPHICS_FASTMALLOC_H

#include <stddef.h>
#include "base/AtomMacros.h"

namespace AtomGraphics {

namespace base {

void *fastMalloc(size_t);

void fastFree(void *);

enum NotNullTag {
    NotNull
};

}

}

#define AT_MAKE_FAST_ALLOCATED_IMPL \
    void* operator new(size_t, void* p) { return p; } \
    void* operator new[](size_t, void* p) { return p; } \
    \
    void* operator new(size_t size) \
    { \
        return ::AtomGraphics::base::fastMalloc(size); \
    } \
    \
    void operator delete(void* p) \
    { \
        ::AtomGraphics::base::fastFree(p); \
    } \
    \
    void* operator new[](size_t size) \
    { \
        return ::AtomGraphics::base::fastMalloc(size); \
    } \
    \
    void operator delete[](void* p) \
    { \
        ::AtomGraphics::base::fastFree(p); \
    } \
    void* operator new(size_t, AtomGraphics::base::NotNullTag, void* location) \
    { \
        ATASSERT(location, "Fast malloc null location"); \
        return location; \
    } \


#define AT_MAKE_FAST_ALLOCATED \
public: \
    AT_MAKE_FAST_ALLOCATED_IMPL \
private: \
typedef int __thisIsHereToForceASemicolonAfterThisMacro

#endif //ATOMGRAPHICS_FASTMALLOC_H
