//
// Created by Amazing on 2018-12-21.
//

#ifndef PODS_ATOMICTIMER_H
#define PODS_ATOMICTIMER_H

#include <mutex>
#include "Timer.h"


namespace AtomGraphics {

class AtomicTimer final : public Timer {

public:
    template<typename TimerFiredClass, typename TimerFiredBaseClass>
    AtomicTimer(TaskRunner *taskRunner, TimerFiredClass &object, void (TimerFiredBaseClass::*function)())
            : Timer(taskRunner, object, function) {}

    AtomicTimer(TaskRunner *taskRunner, std::function<void()> function)
            : Timer(taskRunner, function) {}

    bool isActive() override;

    void fired() override;


private:
    std::mutex m_lock;
};


}


#endif //PODS_ATOMICTIMER_H
