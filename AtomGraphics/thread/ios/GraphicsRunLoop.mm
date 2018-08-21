//
// Created by neo on 2018/7/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsRunLoop.h"

namespace AtomGraphics {

    static CFRunLoopRef globalTimerRunLoop = nullptr;

    void GraphicsRunLoop::setTimerRunLoop(CFRunLoopRef timerRunLoop) {
        globalTimerRunLoop = timerRunLoop;
    }

    CFRunLoopRef GraphicsRunLoop::getTimerRunLoop() {
        return globalTimerRunLoop;
    }

    bool GraphicsRunLoop::isCurrentTimerRunLoop() {
        return CFRunLoopGetCurrent() == globalTimerRunLoop;
    }

}