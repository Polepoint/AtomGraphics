//
// Created by neo on 2019-08-23.
//

#ifndef ATOMGRAPHICS_SCRIPTTIMERCONTROLLER_H
#define ATOMGRAPHICS_SCRIPTTIMERCONTROLLER_H

#include <functional>
#include <map>
#include "ScriptTimer.h"

namespace AtomGraphics {

class ScriptTimerController {

public:

    static ScriptTimerController *Instance();

    static long CreateTimerID(long millionSeconds, bool repeat, const std::function<void()> &function,
                              void *userInfo = nullptr);

    static void RemoveTimerID(long timerID);

private:

    Timer m_timer;
    TimeInterval m_nextScheduleFiredTime{0};
    std::multimap<TimeInterval, std::unique_ptr<ScriptTimer>> m_scriptTimers;

    ScriptTimerController();

    void timerFired();


};

}

#endif //ATOMGRAPHICS_SCRIPTTIMERCONTROLLER_H
