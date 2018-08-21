//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TIMER_H
#define ATOMGRAPHICS_TIMER_H


#include <functional>
#include "base/AtomTimerBase.h"
#include "ThreadTimer.h"

namespace AtomGraphics {

    class Timer final : public TimerBase {

    public:

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

        static void SetThreadTaskRunner(AndroidTaskRunner *taskRunner);

        static bool TimerReady();

#endif

        template<typename TimerFiredClass, typename TimerFiredBaseClass>
        Timer(TimerFiredClass &object, void (TimerFiredBaseClass::*function)())
                : m_function(std::bind(function, &object)) {

        }

        Timer(std::function<void()> function) : m_function(function) {
        }

        void start(ThreadTimerInterval nextFireInterval, ThreadTimerInterval repeatInterval) override;

        void startRepeating(ThreadTimerInterval repeatInterval) override {
            start(repeatInterval, repeatInterval);
        };

        void startOneShot(ThreadTimerInterval delay) override {
            start(delay, 0);
        };

        void stop() override {
            m_validate = false;
            m_repeatInterval = 0;
        };

        bool isActive() override;

        bool isValidate() override {
            return m_validate;
        };

        void fired() override;

        void setNextFireTime(ThreadTimerInterval newTime) override;

        void release();

    private:
        std::function<void()> m_function;


#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS
        friend class TimerHeapLessThanFunction;
        friend class TimerManager;
#endif
    };
}

#endif //ATOMGRAPHICS_TIMER_H
