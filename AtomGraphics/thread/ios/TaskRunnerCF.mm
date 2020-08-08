//
// Created by neo on 2018/8/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <algorithm>
#include "TaskRunnerCF.h"

namespace AtomGraphics {

static const CFTimeInterval kCFTimeIntervalDistantFuture = std::numeric_limits<CFTimeInterval>::max();

TaskRunnerCF::TaskRunnerCF() : m_runLoop(CFRunLoopGetCurrent()) {

}

TaskRunnerCF::~TaskRunnerCF() {
    if (m_timer) {
        CFRunLoopTimerInvalidate(m_timer);
        CFRunLoopRemoveTimer(m_runLoop, m_timer, kCFRunLoopCommonModes);
        CFRelease(m_timer);
    }
}

void TaskRunnerCF::postTask(std::function<void()> task) {
    postScheduleTask(task, Clock::now());
}

void TaskRunnerCF::postScheduleTask(std::function<void()> task, TimeInterval runtime) {
    bool is_next = false;
    {
        std::lock_guard<std::mutex> lock(m_lock);
        auto it = m_tasks.insert(std::make_pair(runtime, std::move(task)));
        if (it == m_tasks.begin())
            is_next = true;
    }
    if (is_next)
        scheduleDelayedWakeUp(runtime);
}

void TaskRunnerCF::scheduleDelayedWakeUp(TimeInterval wakeUpTime) {
    if (wakeUpTime < 0) {
        return;
    }

    CFAbsoluteTime fireDate = CFAbsoluteTimeGetCurrent() + std::max(wakeUpTime - Clock::now(), 0.0);
    if (!m_timer) {
        auto self = this;
        m_timer = CFRunLoopTimerCreateWithHandler(nullptr, fireDate, kCFTimeIntervalDistantFuture, 0, 0, [self](CFRunLoopTimerRef timer) {
            self->runTask(timer);
        });
        CFRunLoopAddTimer(m_runLoop, m_timer, kCFRunLoopCommonModes);
    }
    CFRunLoopTimerSetNextFireDate(m_timer, fireDate);
}

void TaskRunnerCF::runTask(CFRunLoopTimerRef timer) {
    std::function<void()> poppedTask;
    TimeInterval next_wake_up = 0;
    while (next_wake_up < Clock::now()) {
        {
            std::lock_guard<std::mutex> lock(m_lock);
            if (m_tasks.empty())
                return;
            auto it = m_tasks.begin();
            poppedTask = std::move(it->second);
            m_tasks.erase(it);
        }

        runTask(poppedTask);

        if (!m_tasks.empty())
            next_wake_up = m_tasks.begin()->first;
    }

    if (next_wake_up) {
        scheduleDelayedWakeUp(next_wake_up);
    }
}

void TaskRunnerCF::runTask(const std::function<void()> &task) {
    @autoreleasepool {
        task();
    }
}

void TaskRunnerCF::run() {
    if (CFRunLoopGetCurrent() == m_runLoop) {
        CFRunLoopRun();
    }
}
}