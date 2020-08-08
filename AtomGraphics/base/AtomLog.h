//
// Created by neo on 2018/7/31.
//

#ifndef ATOMGRAPHICS_ATOMLOG_H
#define ATOMGRAPHICS_ATOMLOG_H

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <android/log.h>

#endif

namespace AtomGraphics {

#if PLATFORM(ANDROID)

#define ATOM_DEFAULT_TAG "AtomGraphics"

#define AtomLogInfo(fmt, args...) \
{ \
__android_log_print(ANDROID_LOG_INFO, ATOM_DEFAULT_TAG, fmt, ##args); \
}

#define AtomLogError(fmt, args...) \
{ \
__android_log_print(ANDROID_LOG_ERROR, ATOM_DEFAULT_TAG, fmt, ##args); \
}

#endif

#if PLATFORM(IOS)

#if DEBUG

#define AtomLogInfo(fmt, args...) \
{ \
printf(fmt, ##args); \
printf("\n"); \
}

#define AtomLogError(fmt, args...) \
{ \
printf(fmt, ##args); \
printf("\n"); \
}

#else

#define AtomLogInfo(fmt, args...)
#define AtomLogError(fmt, args...)

#endif


void logTimeBegin();

void logTimeEnd();

#endif

}


#endif //ATOMGRAPHICS_ATOMLOG_H
