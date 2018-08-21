//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMTIMERBASE_H
#define ATOMGRAPHICS_ATOMTIMERBASE_H

#include "thread/ThreadTimer.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#import "TimerManager.h"

#endif

namespace AtomGraphics {

    class TimerBase {

    public:

        virtual void start(ThreadTimerInterval nextFireInterval, ThreadTimerInterval repeatInterval) {

        };

        virtual void startRepeating(ThreadTimerInterval repeatInterval) {

        };


        virtual void startOneShot(ThreadTimerInterval delay) {
        };

        virtual void stop() {
        };

        virtual bool isActive() {
            return false;
        };

        virtual bool isValidate() {
            return false;
        };

        virtual void fired() {

        };

        virtual void setNextFireTime(ThreadTimerInterval newTime) {

        };

    protected:
        ThreadTimerInterval m_nextFireTime{0};
        ThreadTimerInterval m_repeatInterval{0};
        ThreadTimerInterval m_previousFiredTime{0};
        unsigned m_heapInsertionOrder{0};
        bool m_validate{true};
        bool m_releaseAfterFired{false};

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS
        friend class TimerManager;
        friend class TimerHeapLessThanFunction;
#endif
    };
}


#endif //ATOMGRAPHICS_ATOMTIMERBASE_H
