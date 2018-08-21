//
// Created by neo on 2018/7/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_THREADTIMER_H
#define ATOMGRAPHICS_THREADTIMER_H

#include "platform/AtomPlatformConfig.h"
#include "GraphicsThread.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

typedef double ThreadTimerInterval;

#endif

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreFoundation/CFDate.h>

typedef CFTimeInterval ThreadTimerInterval;

#endif

namespace AtomGraphics {

    class ThreadTimer {

    public:

        static ThreadTimerInterval now();

        static bool enable();

        static void stop();

        static bool active();

        static void setFireInterval(ThreadTimerInterval interval);

    private:

        static void InitThread();

        friend class GraphicsThread;
    };
}


#endif //ATOMGRAPHICS_THREADTIMER_H
