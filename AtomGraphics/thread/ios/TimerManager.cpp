//
// Created by neo on 2018/7/8.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <limits>
#include <algorithm>
#include <atomic>
#include "TimerManager.h"
#include "GraphicsRunLoop.h"

const ThreadTimerInterval maxDurationOfFiringTimers = 0.05; //50ms

namespace AtomGraphics {

    class TimerHeapLessThanFunction {
    public:
        bool operator()(const TimerBase *, const TimerBase *) const;
    };

    inline bool TimerHeapLessThanFunction::operator()(const TimerBase *a, const TimerBase *b) const {
        // The comparisons below are "backwards" because the heap puts the largest
        // element first and we want the lowest time to be the first one in the heap.
        ThreadTimerInterval aFireTime = a->m_nextFireTime;
        ThreadTimerInterval bFireTime = b->m_nextFireTime;
        if (bFireTime != aFireTime)
            return bFireTime < aFireTime;

        // We need to look at the difference of the insertion orders instead of comparing the two
        // outright in case of overflow.
        unsigned difference = a->m_heapInsertionOrder - b->m_heapInsertionOrder;
        return difference < std::numeric_limits<unsigned>::max() / 2;
    }


    static TimerManager *sharedManager;

    TimerManager *TimerManager::SharedManager() {
        if (!sharedManager) {
            sharedManager = new TimerManager();
        }

        return sharedManager;
    }

    void TimerManager::updateSchedule() {
        if (!ThreadTimer::enable())
            return;

        if (m_firingTimers || m_timerHeap.empty()) {
            m_pendingSharedTimerFireTime = 0;
            ThreadTimer::stop();
        } else {
            ThreadTimerInterval nextFireTime = ((TimerBase *) m_timerHeap.front())->m_nextFireTime;
            ThreadTimerInterval currentTime = ThreadTimer::now();
            if (m_pendingSharedTimerFireTime) {
                // No need to restart the timer if both the pending fire time and the new fire time are in the past.
                if (m_pendingSharedTimerFireTime <= currentTime && nextFireTime <= currentTime)
                    return;
            }
            m_pendingSharedTimerFireTime = nextFireTime;
            ThreadTimer::setFireInterval(std::max(nextFireTime - currentTime, 0.0));
        }
    }

    void TimerManager::fired() {
        if (m_firingTimers)
            return;
        m_firingTimers = true;
        m_pendingSharedTimerFireTime = 0;

        ThreadTimerInterval fireTime = ThreadTimer::now();
        ThreadTimerInterval timeToQuit = fireTime + maxDurationOfFiringTimers;

        while (!m_timerHeap.empty() && ((TimerBase *) m_timerHeap.front())->m_nextFireTime <= fireTime) {
            TimerBase *timer = m_timerHeap.front();
            timer->m_previousFiredTime = timer->m_nextFireTime;
            timer->m_nextFireTime = 0;
            popHeadTimer();

            ThreadTimerInterval interval = timer->m_repeatInterval;
            timer->setNextFireTime(interval ? fireTime + interval : 0);

            // Once the timer has been fired, it may be deleted, so do nothing else with it after this point.
            timer->fired();

            // Catch the case where the timer asked timers to fire in a nested event loop, or we are over time limit.
            if (!m_firingTimers || timeToQuit < ThreadTimer::now())
                break;
        }

        m_firingTimers = false;

        updateSchedule();
    }

    void TimerManager::updateHeapIfNeeded(TimerBase *timer, ThreadTimerInterval oldFireTime) {
        if (timer->m_nextFireTime && checkTimerInHeap(timer))
            return;
        if (!oldFireTime)
            insertTimer(timer);
        else if (!timer->m_nextFireTime)
            removeTimer(timer);
        else if (timer->m_nextFireTime < oldFireTime)
            heapDecreaseKey();

        if (m_timerHeap.size()) {
            TimerBase *headTimer = m_timerHeap.front();
            if (headTimer->m_nextFireTime != m_pendingSharedTimerFireTime) {
                // FIXME: 其他线程启动的timer, 会涉及ThreadTimer::stop()被多次调用的问题，导致闪退
                // 下边能减小ThreadTimer被重复销毁的概率，但是没有根治，需要完善
                if (m_firingTimers && !GraphicsRunLoop::isCurrentTimerRunLoop()) {
                    m_firingTimers = false;
                }
                updateSchedule();
            }
        }
    }

    void TimerManager::removeTimer(TimerBase *timer) {
        auto iter = std::find(m_timerHeap.begin(), m_timerHeap.end(), timer);
        ssize_t index = -1;
        if (iter != m_timerHeap.end())
            index = iter - m_timerHeap.begin();
        auto it = std::next(m_timerHeap.begin(), index);
        m_timerHeap.erase(it);
    }

    void TimerManager::popHeadTimer() {
        std::pop_heap(m_timerHeap.begin(), m_timerHeap.end(), TimerHeapLessThanFunction());
        m_timerHeap.pop_back();
    }


    void TimerManager::insertTimer(TimerBase *timer) {
        static std::atomic<unsigned> currentHeapInsertionOrder;
        timer->m_heapInsertionOrder = currentHeapInsertionOrder++;

        m_timerHeap.push_back(timer);
        heapDecreaseKey();
    }

    bool TimerManager::checkTimerInHeap(TimerBase *timer) {
        auto iter = std::find(m_timerHeap.begin(), m_timerHeap.end(), timer);
        return iter != m_timerHeap.end();
    }


    void TimerManager::heapDecreaseKey() {
        std::push_heap(m_timerHeap.begin(), m_timerHeap.end(), TimerHeapLessThanFunction());
    }
};


