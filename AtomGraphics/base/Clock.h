//
// Created by neo on 2018/11/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TIME_H
#define ATOMGRAPHICS_TIME_H

#include "platform/AtomPlatformConfig.h"
#include "AtomMacros.h"

#if PLATFORM(IOS)

#include <CoreFoundation/CFDate.h>
#include <mach/mach_time.h>
#include <mach/mach_time.h>
#include <mutex>

#elif PLATFORM(ANDROID)

#include <time.h>

#endif

namespace AtomGraphics {

#if PLATFORM(IOS)

typedef CFTimeInterval TimeInterval;

#endif

#if PLATFORM(ANDROID)

typedef double TimeInterval;

#endif


class Clock {

public:

    static TimeInterval now() {
#if PLATFORM(IOS)
        static mach_timebase_info_data_t timebaseInfo;
        static std::once_flag initializeTimerOnceFlag;
        std::call_once(initializeTimerOnceFlag, [] {
            kern_return_t kr = mach_timebase_info(&timebaseInfo);
            ATASSERT(kr == KERN_SUCCESS, "request timer base info error");
        });
        
        return (mach_absolute_time() * timebaseInfo.numer) / (1.0e9 * timebaseInfo.denom);
#endif

#if PLATFORM(ANDROID)
        // TODO: On 32-bit System ...
        struct timespec ts{};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<double>(ts.tv_sec) + ts.tv_nsec / 1.0e9;
#endif
    }
};

}

#endif //ATOMGRAPHICS_TIME_H
