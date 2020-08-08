//
// Created by neo on 2018/8/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TASKRUNNER_H
#define ATOMGRAPHICS_TASKRUNNER_H

#include "base/Clock.h"
#include <functional>

namespace AtomGraphics {

class TaskRunner {

public:

    TaskRunner() {}

    virtual ~TaskRunner() {}

    virtual void postTask(std::function<void()> task) = 0;

    virtual void postScheduleTask(std::function<void()> task, TimeInterval runtime) = 0;

    virtual void run() = 0;
};

}


#endif //ATOMGRAPHICS_TASKRUNNER_H
