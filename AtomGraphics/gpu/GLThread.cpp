//
// Created by neo on 2019/1/22.
//

#include <sys/prctl.h>
#include "base/AtomLog.h"
#include "GLThread.h"

namespace AtomGraphics {

static long idCount_ = 0;

GLThread::GLThread() : m_id(idCount_++) {
    pthread_mutex_init(&m_async_mutex, nullptr);
    pthread_cond_init(&m_async_cond, nullptr);
}

void GLThread::initializeThread() {
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    auto callback = [](void *data) -> void * {
        auto *selfRef = static_cast<GLThread *>(data);
        selfRef->setThreadName();
        selfRef->m_taskRunner = std::make_unique<AndroidTaskRunner>();
        selfRef->triggerTimer();
        selfRef->m_taskRunner->run();
        selfRef->willQuitThread();
        return nullptr;
    };

    int error = pthread_create(&m_thread, &attributes, std::move(callback), this);
    pthread_attr_destroy(&attributes);
    if (error) {
        m_thread = 0;
        AtomLogError("Create GPU Render Thread Error: %d", error);
        return;
    }
}

void GLThread::setThreadName() {
    char threadName[20];
    sprintf(threadName, "AT_GPU_Render%ld", m_id);
    prctl(PR_SET_NAME, threadName);
}

void GLThread::postAsyncRenderTask(std::function<void()> task) {
    enqueueRenderTask(std::move(task), false);
}

void GLThread::postSyncRenderTask(std::function<void()> task) {
    enqueueRenderTask(std::move(task), true);
}

void GLThread::enqueueRenderTask(std::function<void()> task, bool sync) {
    if (m_quit) {
        return;
    }
    if (!m_thread) {
        initializeThread();
    }

    if (sync) {
        auto syncTask = [this, task]() {
            pthread_mutex_lock(&this->m_async_mutex);
            task();
            pthread_cond_signal(&this->m_async_cond);
            pthread_mutex_unlock(&this->m_async_mutex);
        };

        pthread_mutex_lock(&m_async_mutex);
        {
            std::lock_guard<std::mutex> lock(m_lock);
            m_taskQueue.push_back(std::move(syncTask));
        }
        triggerTimer();
        pthread_cond_wait(&m_async_cond, &m_async_mutex);
        pthread_mutex_unlock(&m_async_mutex);
    } else {
        std::lock_guard<std::mutex> lock(m_lock);
        m_taskQueue.push_back(std::move(task));
        triggerTimer();
    }
}

void GLThread::triggerTimer() {
    if (m_taskRunner) {
        if (!m_timer) {
            m_timer = std::make_unique<AtomicTimer>(m_taskRunner.get(), std::bind(&GLThread::executeTasks, this));
        }
        if (!m_isTasksWaitingToBeExecuted) {
            m_isTasksWaitingToBeExecuted = true;
            m_timer->startOneShot(0);
        }
    }
}

void GLThread::executeTasks() {
    m_isTasksWaitingToBeExecuted = false;
    std::vector<std::function<void()>> executingTaskQueue;
    {
        std::lock_guard<std::mutex> lock(m_lock);
        executingTaskQueue.swap(m_taskQueue);
        m_taskQueue.clear();
    }

    for (const auto &task :executingTaskQueue) {
        task();
    }
}

void GLThread::destroy() {
    postAsyncRenderTask([this]() -> void {
        if (!m_thread) {
            return;
        }

        if (m_taskRunner) {
            m_taskRunner->quit();
        }

        m_thread = 0;
    });
    this->m_quit = true;
}

void GLThread::willQuitThread() {
    if (m_quit) {
        pthread_detach(pthread_self());
        m_thread = 0;
        pthread_mutex_destroy(&m_async_mutex);
        pthread_cond_destroy(&m_async_cond);
        delete this;
    }
}

}