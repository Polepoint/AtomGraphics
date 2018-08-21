//
// Created by neo on 2018/7/31.
//

#ifndef ATOMGRAPHICS_ATOMLOG_H
#define ATOMGRAPHICS_ATOMLOG_H

#include "platform/AtomPlatformConfig.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

#include <android/log.h>

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


#endif //ATOMGRAPHICS_ATOMLOG_H
