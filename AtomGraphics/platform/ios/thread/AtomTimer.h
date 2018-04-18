//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMTIMER_H
#define ATOMGRAPHICS_ATOMTIMER_H


#include <functional>
#include "base/AtomTimerBase.h"
#include <CoreFoundation/CFRunLoop.h>

namespace AtomGraphics {

    template<typename TimerFiredClass, typename TimerFiredBaseClass>
    class Timer : public TimerBase {

    public:
        typedef void (TimerFiredClass::*TimerFiredFunction)();

        Timer(TimerFiredClass &object, void (TimerFiredBaseClass::*function)())
                : _function(std::bind(function, &object)) {

        }

        void startOneShot(double delay) override;

        void stop(double delay) override;

        void stop() override;

        void restart() override;

        bool isActive() override;

    private:
        std::function <TimerFiredBaseClass> _function;
        CFRunLoopTimerRef _timer;
    };
}

#endif //ATOMGRAPHICS_ATOMTIMER_H
