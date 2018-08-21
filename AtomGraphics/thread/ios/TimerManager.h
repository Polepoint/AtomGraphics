//
// Created by neo on 2018/7/8.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TIMERMANAGER_H
#define ATOMGRAPHICS_TIMERMANAGER_H

#include "base/AtomTimerBase.h"
#include <vector>


namespace AtomGraphics {

    class TimerBase;

    class TimerManager {

    public:
        static TimerManager *SharedManager();

        void updateSchedule();

        void updateHeapIfNeeded(TimerBase *timer, ThreadTimerInterval oldFireTime);

        bool checkTimerInHeap(TimerBase *timer);

        void fired();

    private:
        std::vector<TimerBase *> m_timerHeap;
        bool m_firingTimers;
        ThreadTimerInterval m_pendingSharedTimerFireTime;
        ThreadTimerInterval m_nextFireTime{0};

        void heapDecreaseKey();

        void removeTimer(TimerBase *timer);

        void popHeadTimer();

        void insertTimer(TimerBase *timer);

        friend class TimerHeapLessThanFunction;

    };

}


#endif //ATOMGRAPHICS_TIMERMANAGER_H
