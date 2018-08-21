//
// Created by neo on 2018/7/26.
//

#include "thread/ThreadTimer.h"
#include <sys/timerfd.h>
#include <pthread.h>
#include <thread>
#include <base/AtomLog.h>
#include <android/looper.h>
#include <thread/GraphicsRunLoop.h>

#define InvalidateTimer -1


inline struct timespec ToPosixTimespec(ThreadTimerInterval seconds) {
    struct timespec ts{};
    const long time_s = static_cast<long>(seconds);
    ts.tv_sec = time_s;
    ts.tv_nsec = static_cast<long>((seconds - time_s) * 1.0e9L);
    return ts;
}


namespace AtomGraphics {

    static int sharedTimer{InvalidateTimer};

    ThreadTimerInterval ThreadTimer::now() {
        // TODO: On 32-bit System ...
        struct timespec ts{};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return static_cast<double>(ts.tv_sec) + ts.tv_nsec / 1.0e9;
    }

    bool ThreadTimer::enable() {
        return GraphicsRunLoop::SharedGraphicsLoop() != nullptr;
    }

    void ThreadTimer::stop() {
        ALooper *looper = GraphicsRunLoop::SharedGraphicsLoop()->getLooper();
        ALooper_removeFd(looper, sharedTimer);
        sharedTimer = InvalidateTimer;
    }

    bool ThreadTimer::active() {
        return sharedTimer != InvalidateTimer;
    }

    bool looperCallbackFiring = false;

    bool hasPendingTimerFD = false;

    int looperCallback(int fd, int events, void *data) {
        if (!(events & (ALOOPER_EVENT_INPUT | ALOOPER_EVENT_ERROR |
                        ALOOPER_EVENT_HANGUP | ALOOPER_EVENT_INVALID))) {
            return 1;
        }
        looperCallbackFiring = true;
        looperCallbackFiring = false;
        int keepFD = hasPendingTimerFD ? 1 : 0;
        hasPendingTimerFD = false;
        if (!keepFD) {
            GraphicsRunLoop::SharedGraphicsLoop()->quit();
        }
        return keepFD;
    }

    void ThreadTimer::setFireInterval(ThreadTimerInterval interval) {
        if (sharedTimer == InvalidateTimer) {
            sharedTimer = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
            ALooper *looper = GraphicsRunLoop::SharedGraphicsLoop()->getLooper();
            int error = ALooper_addFd(looper, sharedTimer, ALOOPER_POLL_CALLBACK,
                                      ALOOPER_EVENT_INPUT | ALOOPER_EVENT_ERROR |
                                      ALOOPER_EVENT_HANGUP, looperCallback, nullptr);
            if (error == -1) {
                AtomLogError("Add TimerFd Error");
            }
        }

        struct itimerspec wake_up = {};
        wake_up.it_value = ToPosixTimespec(interval + now());
        int error = timerfd_settime(sharedTimer, TFD_TIMER_ABSTIME, &wake_up, nullptr);
        if (error == -1) {
            AtomLogError("SetTimer Error: %d", error);
        }
        hasPendingTimerFD = looperCallbackFiring;
    }

    void ThreadTimer::InitThread() {
        GraphicsRunLoop::InitSharedGraphicsLoop();
    }
}