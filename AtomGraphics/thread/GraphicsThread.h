//
// Created by neo on 2018/7/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSTHREAD_H
#define ATOMGRAPHICS_GRAPHICSTHREAD_H

#include <functional>
#include "platform/AtomPlatformConfig.h"
#include "TaskRunner.h"

namespace AtomGraphics {

class GraphicsThread {

public:

    static TaskRunner *GraphicsThreadTaskRunner();

    static bool IsGraphicsThread();

    static void DispatchOnGraphicsQueue(std::function<void()> block);

#if PLATFORM(ANDROID)

    static void DispatchOnGraphicsQueueWithJSContextScope(const std::function<void()>& block);

#endif

};

}


#endif //ATOMGRAPHICS_GRAPHICSTHREAD_H
