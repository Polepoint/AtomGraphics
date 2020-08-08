
#ifndef __BASE_ATOMMACROS_H__
#define __BASE_ATOMMACROS_H__

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(IOS)

#include <assert.h>

#define AG_ASSERT(cond) assert(cond)

#endif


#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#ifndef ATASSERT
#if defined(DEBUG)
#define ATASSERT(cond, msg) AG_ASSERT(cond)
#else
#define ATASSERT(cond, msg)
#endif
#endif

#define GP_ASSERT(cond) ATASSERT(cond, "")

#define AT_MAKE_NONCOPYABLE(ClassName) \
    private: \
        ClassName(const ClassName&) = delete; \
        ClassName& operator=(const ClassName&) = delete; \

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))


#endif //__BASE_ATOMMACROS_H__
