//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <map>
#include "thread/GraphicsThread.h"
#include "ScriptTimer.h"

namespace AtomGraphics {

static long s_globalScriptTimerID = 0;

ScriptTimer::ScriptTimer(long millionSeconds, bool repeat, const std::function<void()> &function)
        : m_timerID(s_globalScriptTimerID++), m_repeats(repeat), m_function(function) {
    m_schedule_time = millionSeconds / 1000.0 + Clock::now();
    if (repeat) {
        m_interval_time = millionSeconds / 1000.0;
    }
}

void ScriptTimer::fired() {
    m_function();
}

}
