//
// Created by neo on 2018/8/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TASKRUNNERCF_H
#define ATOMGRAPHICS_TASKRUNNERCF_H

#import <JavaScriptCore/JavaScriptCore.h>
#import <functional>
#import <map>
#import <mutex>
#import "TaskRunner.h"
#import "Timer.h"

namespace AtomGraphics {

class TaskRunnerCF final : public TaskRunner {

public:

    TaskRunnerCF();

    ~TaskRunnerCF();

    void postTask(std::function<void()> task) override;

    void postScheduleTask(std::function<void()> task, TimeInterval runtime) override;

    void run() override;

private:

    std::mutex m_lock;
    CFRunLoopRef m_runLoop;
    std::multimap<double, std::function<void()>> m_tasks;
    CFRunLoopTimerRef m_timer{nullptr};

    void scheduleDelayedWakeUp(TimeInterval wakeUpTime);

    void runTask(CFRunLoopTimerRef);

    void runTask(const std::function<void()> &task);

};

}


#endif //ATOMGRAPHICS_TASKRUNNERCF_H
