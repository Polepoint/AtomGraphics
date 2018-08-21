//
// Created by neo on 2018/7/10.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_SCRIPTTIMER_H
#define ATOMGRAPHICS_SCRIPTTIMER_H


#include "thread/Timer.h"

namespace AtomGraphics {

    class ScriptTimer {
    public:

        static long CreateTimerID(long millionSeconds, bool repeat, const std::function<void()> &function);

        static void RemoveTimerID(long timerID);

        ScriptTimer(long millionSeconds, bool repeat, const std::function<void()> &function);

        virtual ~ScriptTimer();

    private:
        long m_timerID;
        bool m_repeats;
        Timer *m_timer;
        std::function<void()> m_function;

        void fired();
    };
};


#endif //ATOMGRAPHICS_SCRIPTTIMER_H
