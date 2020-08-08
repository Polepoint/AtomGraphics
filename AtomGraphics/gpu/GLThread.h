//
// Created by neo on 2019/1/22.
//

#ifndef ATOMGRAPHICS_GLTHREAD_H
#define ATOMGRAPHICS_GLTHREAD_H


#include <sys/types.h>
#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include "thread/AtomicTimer.h"
#include "thread/android/AndroidTaskRunner.h"

namespace AtomGraphics {

class GLThread final {

public:

    GLThread();

    void postAsyncRenderTask(std::function<void()> task);

    void postSyncRenderTask(std::function<void()> task);

    void destroy();

private:

    long m_id;

    std::unique_ptr<AndroidTaskRunner> m_taskRunner;
    std::unique_ptr<AtomicTimer> m_timer;
    std::vector<std::function<void()>> m_taskQueue;
    std::atomic_bool m_isTasksWaitingToBeExecuted{false};

    pthread_t m_thread{0};
    pthread_cond_t m_async_cond{0};
    pthread_mutex_t m_async_mutex{0};

    std::mutex m_lock;

    std::atomic_bool m_quit{false};

    ~GLThread() = default;

    void initializeThread();

    void setThreadName();

    void triggerTimer();

    void executeTasks();

    void enqueueRenderTask(std::function<void()> task, bool sync);

    void willQuitThread();

};

}


#endif //ATOMGRAPHICS_GLTHREAD_H
