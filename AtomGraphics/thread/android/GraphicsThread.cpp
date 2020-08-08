//
// Created by neo on 2018/7/27.
//

#include <pthread.h>
#include <sched.h>
#include <vector>
#include <v8/v8.h>


#include "javascript/v8/V8JavaScriptInterface.h"
#include "javascript/v8/V8JSContext.h"
#include "javascript/v8/V8Platform.h"
#include "base/AtomLog.h"
#include "thread/AtomicTimer.h"
#include "thread/GraphicsThread.h"
#include "AndroidTaskRunner.h"

namespace AtomGraphics {

using namespace JS;

static pthread_t runningGraphicsThread = 0;

static std::vector<std::function<void()>> pendingInvokeFunctions;

static AtomicTimer *taskRunnerTimer;

static AndroidTaskRunner *GraphicsTaskRunner;

static std::mutex dispatch_lock;

static void DispatchTask() {
    std::vector<std::function<void()>> functions;
    {
        std::lock_guard<std::mutex> lock(dispatch_lock);
        functions.swap(pendingInvokeFunctions);
        pendingInvokeFunctions.clear();
    }
    for (const auto &func : functions) {
        func();
    }
}


inline void pushTask(std::function<void()> block) {
    std::lock_guard<std::mutex> lock(dispatch_lock);
    pendingInvokeFunctions.push_back(std::move(block));
}

void GraphicsThread::DispatchOnGraphicsQueue(std::function<void()> block) {
    if (runningGraphicsThread == 0) {
        pushTask(std::move(block));
        pthread_attr_t attributes;
        pthread_attr_init(&attributes);
        auto callback = [](void *) -> void * {
            v8::Isolate *isolate = V8Platform::SharedPlatform()->getIsolate();
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);
            GraphicsTaskRunner = new AndroidTaskRunner();
            DispatchTask();
            GraphicsTaskRunner->run();
            delete GraphicsTaskRunner;
            GraphicsTaskRunner = nullptr;
            /* TODO 现在线程结束得比较快，之后又可能立即创建，不确定是否会影响性能
             * 可以考虑使用pthread_cond_wait，激活Graphics线程
             */
            pthread_detach(pthread_self());
            runningGraphicsThread = 0;
            return nullptr;
        };
        int error = pthread_create(&runningGraphicsThread, &attributes, std::move(callback), nullptr);
        pthread_attr_destroy(&attributes);
        if (error) {
            runningGraphicsThread = 0;
            AtomLogError("Create Thread Error: %d", error);
        }
    } else {
        pushTask(std::move(block));
        if (GraphicsTaskRunner) {
            if (!taskRunnerTimer) {
                taskRunnerTimer = new AtomicTimer(GraphicsTaskRunner, &DispatchTask);
            }
            if (IsGraphicsThread() || !taskRunnerTimer->isActive()) {
                taskRunnerTimer->startOneShot(0);
            }
        }
    }
}

void GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope(const std::function<void()>& block) {
    DispatchOnGraphicsQueue([block]() -> void {
        v8::Isolate *isolate = V8Platform::SharedPlatform()->getIsolate();
        v8::Context::Scope contextScope(
                V8JavaScriptInterface::SharedInterface()->context()->getGlobalContext().Get(isolate));
        block();
    });
}

TaskRunner *GraphicsThread::GraphicsThreadTaskRunner() {
    return GraphicsTaskRunner;
}

bool GraphicsThread::IsGraphicsThread() {
    return pthread_equal(pthread_self(), runningGraphicsThread) != 0;
}

}