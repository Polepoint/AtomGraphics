//
// Created by neo on 2018/8/7.
//

#ifndef ATOMGRAPHICS_GRAPHICSTASKRUNNER_H
#define ATOMGRAPHICS_GRAPHICSTASKRUNNER_H

#include <android/looper.h>
#include <mutex>
#include <map>
#include <deque>
#include <thread/ThreadTimer.h>

namespace AtomGraphics {

    class ThreadTimer;

    class AndroidTaskRunner {

    public:
        AndroidTaskRunner();

        virtual ~AndroidTaskRunner();

        void run();

        void quit();

        void postScheduleTask(std::function<void()> task, double runtime);

        void postTask(std::function<void()> task);

    private:
        ALooper *m_looper;
        std::mutex m_lock;

        int m_timerFD;
        int m_eventFD;

        std::map<int, std::function<void()>> m_watch_tasks;
        std::deque<std::function<void()>> m_immediate_tasks;
        std::multimap<double, std::function<void()>> m_delayed_tasks;

        bool m_quit;

        void runImmediateTask();

        void runDelayedTask();

        void addFileDescriptorWatch(int fd, std::function<void()> task);

        bool onFileDescriptorEvent(int, int);

        static void runTask(const std::function<void()> &task);

        void scheduleDelayedWakeUp(double time);

        void scheduleImmediateWakeUp();
    };

}

#endif //ATOMGRAPHICS_GRAPHICSTASKRUNNER_H
