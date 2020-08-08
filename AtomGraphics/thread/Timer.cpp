//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "Timer.h"
#include "GraphicsThread.h"

namespace AtomGraphics {

void Timer::start(TimeInterval nextFireInterval, TimeInterval repeatInterval) {
    m_repeatInterval = repeatInterval;
    setNextFireTime(nextFireInterval + Clock::now());
}

bool Timer::isActive() {
    return m_nextFireTime > 0;
}

void Timer::fired() {
    if (m_validate) {
        m_function();
        if (m_repeatInterval > 0) {
            setNextFireTime(m_repeatInterval + Clock::now());
        } else {
            m_nextFireTime = 0;
        }
    }
}

void Timer::setNextFireTime(TimeInterval newTime) {
    // Keep heap valid while changing the next-fire time.
    TimeInterval oldTime = m_nextFireTime;

    if (!m_taskRunner) {
        return;
    }

    if (oldTime != newTime) {
        m_nextFireTime = newTime;
        m_taskRunner->postScheduleTask(std::bind(&TimerTask::onTask, m_task), newTime);
        m_task->requireActive();
    }
}

}