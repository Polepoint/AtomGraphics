//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <map>
#include "ScriptTimer.h"

namespace AtomGraphics {

    static std::map<long, ScriptTimer *> registeredTimers;

    long ScriptTimer::CreateTimerID(long millionSeconds, bool repeat, const std::function<void()> &function) {
        ScriptTimer *scriptTimer = new ScriptTimer(millionSeconds, repeat, function);
        registeredTimers[scriptTimer->m_timerID] = scriptTimer;
        return scriptTimer->m_timerID;
    }

    void ScriptTimer::RemoveTimerID(long timerID) {
        ScriptTimer *timer = registeredTimers[timerID];
        if (timer) {
            registeredTimers.erase(timerID);
            if (timer->m_timer->isActive()) {
                timer->m_timer->stop();
            }
            delete timer;
        }
    }

    ScriptTimer::ScriptTimer(long millionSeconds, bool repeats, const std::function<void()> &function)
            : m_repeats(repeats), m_function(function) {
        static long globalScriptTimerID = 0;
        m_timerID = globalScriptTimerID++;
        m_timer = new Timer(*this, &ScriptTimer::fired);
        ThreadTimerInterval interval = millionSeconds / 1000.0;
        if (repeats) {
            m_timer->startRepeating(interval);
        } else {
            m_timer->startOneShot(interval);
        }
    }

    ScriptTimer::~ScriptTimer() {
        m_timer->release();
    }

    void ScriptTimer::fired() {
        m_function();
        if (!m_repeats) {
            RemoveTimerID(this->m_timerID);
        }
    }
}
