//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TIMER_H
#define ATOMGRAPHICS_TIMER_H


#include <functional>
#include "base/Clock.h"
#include "TimerBase.h"
#include "TaskRunner.h"

namespace AtomGraphics {

class Timer : public TimerBase {

private:

    class TimerTask {
    public:

        TimerTask(Timer *mTimer) : m_timer(mTimer) {}

        void invalidate() {
            if (m_active) {
                m_alive = false;
            } else {
                delete this;
            }
        }

        void onTask() {
            if (!m_alive) {
                delete this;
                return;
            }

            m_timer->fired();
            inactive();
        }

        void requireActive() {
            m_active = true;
        }

        void inactive() {
            m_active = false;
        }

    private:

        bool m_alive{true};
        bool m_active{false};
        Timer *m_timer;
    };

public:

    template<typename TimerFiredClass, typename TimerFiredBaseClass>
    Timer(TaskRunner *taskRunner, TimerFiredClass &object, void (TimerFiredBaseClass::*function)())
            :m_taskRunner(taskRunner),
             m_function(std::bind(function, &object)),
             m_task(new TimerTask(this)) {}

    Timer(TaskRunner *taskRunner, std::function<void()> function)
            : m_taskRunner(taskRunner),
              m_function(function),
              m_task(new TimerTask(this)) {}

    virtual ~Timer() {
        m_task->invalidate();
    }

    void start(TimeInterval nextFireInterval, TimeInterval repeatInterval) override;

    void startRepeating(TimeInterval repeatInterval) override {
        start(repeatInterval, repeatInterval);
    };

    void startOneShot(TimeInterval delay) override {
        start(delay, 0);
    };

    void stop() override {
        m_validate = false;
        m_repeatInterval = 0;
        m_task->inactive();
    };

    bool isActive() override;

    void fired() override;

    void setNextFireTime(TimeInterval newTime) override;

private:
    TaskRunner *m_taskRunner;
    std::function<void()> m_function;
    TimerTask *m_task;
};
}

#endif //ATOMGRAPHICS_TIMER_H
