//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "Timer.h"

namespace AtomGraphics {

    void Timer::start(ThreadTimerInterval nextFireInterval, ThreadTimerInterval repeatInterval) {
        m_repeatInterval = repeatInterval;
        setNextFireTime(ThreadTimer::now() + nextFireInterval);
    }

    bool Timer::isActive() {
        return m_nextFireTime > 0;
    }

    void Timer::fired() {
        if (m_validate) {
            m_function();
        } else if (m_releaseAfterFired) {
            delete this;
        }
    }

    void Timer::setNextFireTime(ThreadTimerInterval newTime) {
        // Keep heap valid while changing the next-fire time.
        ThreadTimerInterval oldTime = m_nextFireTime;

        if (oldTime != newTime) {
            m_nextFireTime = newTime;

            TimerManager::SharedManager()->updateHeapIfNeeded(this, oldTime);
        }
    }

    void Timer::release() {
        this->stop();
        if (this->isActive()) {
            m_releaseAfterFired = true;
        } else {
            delete this;
        }
    }
}