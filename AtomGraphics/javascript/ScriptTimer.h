//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_SCRIPTTIMER_H
#define ATOMGRAPHICS_SCRIPTTIMER_H


#include "thread/Timer.h"
#include <memory>

namespace AtomGraphics {

class ScriptTimer {

public:

    static std::unique_ptr<ScriptTimer> CreateTimer(long millionSeconds, bool repeat,
                                                    const std::function<void()> &function, void *userInfo);

    ScriptTimer(long millionSeconds, bool repeat, const std::function<void()> &function);

    virtual ~ScriptTimer() = default;

private:

    long m_timerID;
    bool m_repeats;
    bool m_cancel{false};
    TimeInterval m_schedule_time;
    TimeInterval m_interval_time;
    std::function<void()> m_function;

    void fired();

    friend class ScriptTimerController;
};

};


#endif //ATOMGRAPHICS_SCRIPTTIMER_H
