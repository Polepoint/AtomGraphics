//
// Created by neo on 2018/7/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsThread.h"
#import <dispatch/dispatch.h>
#import "ThreadTimer.h"
#import "GraphicsRunLoop.h"
#import "Timer.h"

#define DISPATCH_GRAPHICS_QUEUE_LABEL "com.neo.AtomGraphics"

namespace AtomGraphics {


    static dispatch_queue_t graphics_queue;

    static Timer *eventLoopDispatchTimer = nullptr;

    static std::vector<std::function<void()>> eventLoopDispatchBlocks;

    static void OnEventLoop() {
        auto blocksCopy = eventLoopDispatchBlocks;
        eventLoopDispatchBlocks.clear();
        for (auto block : blocksCopy) {
            block();
        }
    }

    void GraphicsThread::InitGraphicsThread() {
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^() {
            graphics_queue = dispatch_queue_create(DISPATCH_GRAPHICS_QUEUE_LABEL, DISPATCH_QUEUE_SERIAL);
            DispatchOnGraphicsQueue(^() {
                GraphicsThreadEnable();
            });
        });
    }

    void GraphicsThread::DispatchOnGraphicsQueue(std::function<void()> block) {
        dispatch_queue_t currentQueue = dispatch_get_current_queue();
        if (strcmp(dispatch_queue_get_label(currentQueue), DISPATCH_GRAPHICS_QUEUE_LABEL) == 0) {
            block();
        } else if (ThreadTimer::active()) {
            if (!eventLoopDispatchTimer) {
                eventLoopDispatchTimer = new Timer(OnEventLoop);
            }
            eventLoopDispatchBlocks.push_back(block);
            if (!eventLoopDispatchTimer->isActive()) {
                eventLoopDispatchTimer->startOneShot(0);
            }
        } else {
            dispatch_async(graphics_queue, ^() {
                GraphicsRunLoop::setTimerRunLoop(CFRunLoopGetCurrent());
                block();
                CFRunLoopRun();
            });
        }
    }

    void GraphicsThread::GraphicsThreadEnable() {
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^() {
            ThreadTimer::InitThread();
        });
    }

}

