
#ifndef __BASE_ATOMMACROS_H__
#define __BASE_ATOMMACROS_H__

#include "platform/ios/AtomPlatformDefine-ios.h"


#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#ifndef ATASSERT
#if COCOS2D_DEBUG > 0
#if CC_ENABLE_SCRIPT_BINDING
    extern bool CC_DLL cc_assert_script_compatible(const char *msg);
    #define ATASSERT(cond, msg) do {                              \
          if (!(cond)) {                                          \
            if (!cc_assert_script_compatible(msg) && strlen(msg)) \
              cocos2d::log("Assert failed: %s", msg);             \
            AG_ASSERT(cond);                                      \
          } \
        } while (0)
    #else
    #define ATASSERT(cond, msg) AG_ASSERT(cond)
    #endif
#else
#define ATASSERT(cond, msg)
#endif
#endif

#define GP_ASSERT(cond) ATASSERT(cond, "")

#endif
