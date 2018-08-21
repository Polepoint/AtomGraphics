//
// Created by neo on 2018/7/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#import <mach/mach_time.h>
#import <mutex>
#import "ThreadTimer.h"
#import "TimerManager.h"
#import "AtomMacros.h"
#import "GraphicsRunLoop.h"


namespace AtomGraphics {

    static bool threadInitialized{false};

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS
    static CFRunLoopTimerRef sharedTimer;
    static const CFTimeInterval kCFTimeIntervalDistantFuture = std::numeric_limits<CFTimeInterval>::max();
#endif

    ThreadTimerInterval ThreadTimer::now() {
        static mach_timebase_info_data_t timebaseInfo;
        static std::once_flag initializeTimerOnceFlag;
        std::call_once(initializeTimerOnceFlag, [] {
            kern_return_t kr = mach_timebase_info(&timebaseInfo);
            ATASSERT(kr == KERN_SUCCESS, "request timer base info error");
        });

        return (mach_absolute_time() * timebaseInfo.numer) / (1.0e9 * timebaseInfo.denom);
    }

    bool ThreadTimer::enable() {
        return threadInitialized;
    }

    void ThreadTimer::stop() {
        if (!sharedTimer)
            return;

        CFRunLoopTimerInvalidate(sharedTimer);
        CFRelease(sharedTimer);
        sharedTimer = nullptr;
    }

    bool ThreadTimer::active() {
        return sharedTimer != nullptr;;
    }

    void timerFired(CFRunLoopTimerRef, void *) {
        TimerManager::SharedManager()->fired();
    }

    void ThreadTimer::setFireInterval(ThreadTimerInterval interval) {
        CFAbsoluteTime fireDate = CFAbsoluteTimeGetCurrent() + interval;
        if (!sharedTimer) {
            sharedTimer = CFRunLoopTimerCreate(nullptr, fireDate, kCFTimeIntervalDistantFuture, 0, 0, timerFired, nullptr);
            CFRunLoopAddTimer(GraphicsRunLoop::getTimerRunLoop(), sharedTimer, kCFRunLoopDefaultMode);
        }
        CFRunLoopTimerSetNextFireDate(sharedTimer, fireDate);
    }

    void ThreadTimer::InitThread() {
        threadInitialized = true;
    }
}
