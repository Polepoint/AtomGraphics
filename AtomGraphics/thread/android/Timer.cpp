//
// Created by neo on 2018/8/11.
//

#include "thread/Timer.h"


namespace AtomGraphics {

    static AndroidTaskRunner *threadTaskRunner = nullptr;

    void Timer::SetThreadTaskRunner(AndroidTaskRunner *taskRunner) {
        threadTaskRunner = taskRunner;
    }

    bool Timer::TimerReady() {
        return threadTaskRunner != nullptr;
    }

    void Timer::start(ThreadTimerInterval nextFireInterval, ThreadTimerInterval repeatInterval) {
        m_repeatInterval = repeatInterval;
        setNextFireTime(ThreadTimer::now() + nextFireInterval);
    }

    bool Timer::isActive() {
        return m_nextFireTime > 0;
    }

    void Timer::fired() {
        m_nextFireTime = 0;
        if (m_validate) {
            m_function();
            if (m_repeatInterval > 0) {
                setNextFireTime(ThreadTimer::now() + m_repeatInterval);
            }
        } else if (m_releaseAfterFired) {
            delete this;
        }
    }

    void Timer::setNextFireTime(ThreadTimerInterval newTime) {
        ThreadTimerInterval oldTime = m_nextFireTime;

        if (oldTime != newTime && threadTaskRunner) {
            m_nextFireTime = newTime;
            threadTaskRunner->postScheduleTask(std::bind(&Timer::fired, this), m_nextFireTime);
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