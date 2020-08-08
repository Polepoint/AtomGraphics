//
// Created by neo on 2018/7/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#import "GraphicsThread.h"
#import <vector>
#import "AtomicTimer.h"
#import "TaskRunnerCF.h"

#define DISPATCH_GRAPHICS_QUEUE_LABEL "com.neo.AtomGraphics"

namespace AtomGraphics {


static dispatch_queue_t sGraphicsQueue;

static std::vector<std::function<void()>> sEventLoopDispatchBlocks;

static std::unique_ptr<AtomicTimer> sEventLoopDispatchTimer;

static std::unique_ptr<TaskRunnerCF> sGraphicsTaskRunner;

static std::mutex sTaskLock;

static void DispatchTask() {
    std::vector<std::function<void()>> blocksCopy;
    {
        std::lock_guard<std::mutex> lock(sTaskLock);
        blocksCopy.assign(sEventLoopDispatchBlocks.begin(), sEventLoopDispatchBlocks.end());
        sEventLoopDispatchBlocks.clear();
    };

    for (auto &block : blocksCopy) {
        block();
    }
}

TaskRunner *GraphicsThread::GraphicsThreadTaskRunner() {
    return sGraphicsTaskRunner.get();
}

static void addBlock(std::function<void()> block) {
    std::lock_guard<std::mutex> lock(sTaskLock);
    sEventLoopDispatchBlocks.push_back(block);
}

void GraphicsThread::DispatchOnGraphicsQueue(std::function<void()> block) {
    if (!sGraphicsQueue) {
        addBlock(std::move(block));

        dispatch_queue_attr_t queue_attr = DISPATCH_QUEUE_SERIAL;
        queue_attr = dispatch_queue_attr_make_with_qos_class(queue_attr, QOS_CLASS_USER_INTERACTIVE, -1);
        sGraphicsQueue = dispatch_queue_create(DISPATCH_GRAPHICS_QUEUE_LABEL, queue_attr);

        dispatch_async(sGraphicsQueue, ^() {
            sGraphicsTaskRunner.reset(new TaskRunnerCF());
            sEventLoopDispatchTimer.reset(new AtomicTimer(sGraphicsTaskRunner.get(), &DispatchTask));
            sEventLoopDispatchTimer->startOneShot(0);
            sGraphicsTaskRunner->run();
            sGraphicsTaskRunner.reset();
            sEventLoopDispatchTimer.reset();
            sGraphicsQueue = NULL;
        });
    } else {
        addBlock(std::move(block));
        if (IsGraphicsThread() || (sEventLoopDispatchTimer && !sEventLoopDispatchTimer->isActive())) {
            sEventLoopDispatchTimer->startOneShot(0);
        }
    }
}

bool GraphicsThread::IsGraphicsThread() {
    return dispatch_get_current_queue() == sGraphicsQueue;
}

}

