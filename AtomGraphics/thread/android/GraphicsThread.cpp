//
// Created by neo on 2018/7/27.
//

#include "thread/GraphicsThread.h"
#include "thread/ThreadTimer.h"
#include <pthread.h>
#include <base/AtomLog.h>
#include <thread/GraphicsRunLoop.h>
#include <vector>
#include "thread/Timer.h"

namespace AtomGraphics {

    static pthread_t runningGraphicsThread = 0;

    static std::vector<std::function<void()>> pendingInvokeFunctions;

    static Timer *taskRunnerTimer;

    static std::mutex dispatch_lock;

    AndroidTaskRunner *UITaskRunner{nullptr};

    static void DispatchTask() {
        std::vector<std::function<void()>> functions;
        {
            std::lock_guard<std::mutex> lock(dispatch_lock);
            functions = pendingInvokeFunctions;
            pendingInvokeFunctions.clear();
        }
        for (auto func : functions) {
            func();
        }
    }

    void GraphicsThread::InitGraphicsThread() {
        if (!runningGraphicsThread) {

        }
    }

    void GraphicsThread::DispatchOnGraphicsQueue(std::function<void()> block) {
        if (runningGraphicsThread == 0) {
            {
                std::lock_guard<std::mutex> lock(dispatch_lock);
                pendingInvokeFunctions.push_back(block);
            }
            pthread_attr_t attributes;
            pthread_attr_init(&attributes);
            auto callback = [](void *) -> void * {
                AndroidTaskRunner *threadTaskRunner = new AndroidTaskRunner();
                Timer::SetThreadTaskRunner(threadTaskRunner);
                DispatchTask();
                threadTaskRunner->run();
                /* TODO 现在线程结束得比较快，之后又可能立即创建，不确定是否会影响性能
                 * 可以考虑使用pthread_cond_wait，激活Graphics线程
                 */
                pthread_detach(pthread_self());
                runningGraphicsThread = 0;
                return nullptr;
            };
            int error = pthread_create(&runningGraphicsThread, &attributes, std::move(callback), NULL);
            pthread_attr_destroy(&attributes);
            if (error) {
                runningGraphicsThread = 0;
                AtomLogError("Create Thread Error: %d", error);
            }
        } else if (!pthread_equal(pthread_self(), runningGraphicsThread)) {
            {
                std::lock_guard<std::mutex> lock(dispatch_lock);
                pendingInvokeFunctions.push_back(block);
            }
            if (Timer::TimerReady()) {
                if (!taskRunnerTimer) {
                    taskRunnerTimer = new Timer(&DispatchTask);
                }
                if (!taskRunnerTimer->isActive()) {
                    taskRunnerTimer->startOneShot(0);
                } else {
                    AtomLogError("timer inactive");
                }
            } else {
                AtomLogError("timer unavailable, timer active: %d", ThreadTimer::active());
            }
        } else {
            block();
        }
    }

    void GraphicsThread::GraphicsThreadEnable() {
        ThreadTimer::InitThread();
    }

    void GraphicsThread::InitMainThread() {
        if (!UITaskRunner) {
            UITaskRunner = new AndroidTaskRunner();
        }
    }

    AndroidTaskRunner *GraphicsThread::uiTaskRunner() {
        return UITaskRunner;
    }
}