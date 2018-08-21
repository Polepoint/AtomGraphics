//
// Created by neo on 2018/8/7.
//

#include "AndroidTaskRunner.h"
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <base/AtomLog.h>
#include <thread/ThreadTimer.h>
#include <unistd.h>

inline struct timespec ToPosixTimespec(ThreadTimerInterval seconds) {
    struct timespec ts{};
    const long time_s = static_cast<long>(seconds);
    ts.tv_sec = time_s;
    ts.tv_nsec = static_cast<long>((seconds - time_s) * 1.0e9L);
    return ts;
}


namespace AtomGraphics {

    AndroidTaskRunner::AndroidTaskRunner() :
            m_looper(ALooper_prepare(0)),
            m_eventFD(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)),
            m_timerFD(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)) {
        ALooper_acquire(m_looper);
        addFileDescriptorWatch(m_eventFD, std::bind(&AndroidTaskRunner::runImmediateTask, this));
        addFileDescriptorWatch(m_timerFD, std::bind(&AndroidTaskRunner::runDelayedTask, this));
    }

    AndroidTaskRunner::~AndroidTaskRunner() {
        ALooper_release(m_looper);
        std::lock_guard<std::mutex> lock(m_lock);
        for (const auto &watch : m_watch_tasks) {
            // ALooper doesn't guarantee that each watch doesn't run one last time if
            // the file descriptor was already signalled. To guard against this point
            // the watch to a no-op callback.
            ALooper_addFd(m_looper, watch.first, ALOOPER_POLL_CALLBACK,
                          ALOOPER_EVENT_INPUT | ALOOPER_EVENT_ERROR | ALOOPER_EVENT_HANGUP,
                          [](int, int, void *) -> int { return 0; }, nullptr);
            ALooper_removeFd(m_looper, watch.first);
        }
        ALooper_release(m_looper);

        struct itimerspec time = {};
        timerfd_settime(m_timerFD, TFD_TIMER_ABSTIME, &time, nullptr);
    }

    void AndroidTaskRunner::run() {
        m_quit = false;
        while (true) {
            {
                std::lock_guard<std::mutex> lock(m_lock);
                if (m_quit)
                    break;
            }
            ALooper_pollOnce(-1 /* timeout */, nullptr, nullptr, nullptr);
        }
    }

    void AndroidTaskRunner::quit() {
        std::lock_guard<std::mutex> lock(m_lock);
        m_quit = true;
        ALooper_wake(m_looper);
    }

    void AndroidTaskRunner::scheduleDelayedWakeUp(ThreadTimerInterval time) {
        if (time <= 0) {
            return;
        }
        struct itimerspec wake_up = {};
        wake_up.it_value = ToPosixTimespec(time);
        if (timerfd_settime(m_timerFD, TFD_TIMER_ABSTIME, &wake_up,
                            nullptr) == -1) {
            AtomLogError("timerfd_settime error");
        }
    }

    void AndroidTaskRunner::scheduleImmediateWakeUp() {
        uint64_t value = 1;
        if (write(m_eventFD, &value, sizeof(value)) == -1 &&
            errno != EAGAIN) {
            AtomLogError("write failure");
        }
    }

    void AndroidTaskRunner::postTask(std::function<void()> task) {
        bool was_empty;
        {
            std::lock_guard<std::mutex> lock(m_lock);
            was_empty = m_immediate_tasks.empty();
            m_immediate_tasks.push_back(std::move(task));
        }
        if (was_empty)
            scheduleImmediateWakeUp();
    }

    void AndroidTaskRunner::postScheduleTask(std::function<void()> task, ThreadTimerInterval runtime) {
        bool is_next = false;
        {
            std::lock_guard<std::mutex> lock(m_lock);
            auto it = m_delayed_tasks.insert(std::make_pair(runtime, std::move(task)));
            if (it == m_delayed_tasks.begin())
                is_next = true;
        }
        if (is_next)
            scheduleDelayedWakeUp(runtime);
    }

    void AndroidTaskRunner::addFileDescriptorWatch(int fd, std::function<void()> task) {
        if (!fd) {
            return;
        }
        std::lock_guard<std::mutex> lock(m_lock);
        if (m_watch_tasks.count(fd)) {
            return;
        }
        m_watch_tasks[fd] = std::move(task);
        // It's safe for the callback to hang on to |this| as everything is
        // unregistered in the destructor.
        auto callback = [](int signalled_fd, int events, void *data) -> int {
            AndroidTaskRunner *task_runner = reinterpret_cast<AndroidTaskRunner *>(data);
            return task_runner->onFileDescriptorEvent(signalled_fd, events) ? 1 : 0;
        };
        if (ALooper_addFd(m_looper, fd, ALOOPER_POLL_CALLBACK,
                          ALOOPER_EVENT_INPUT | ALOOPER_EVENT_ERROR |
                          ALOOPER_EVENT_HANGUP,
                          std::move(callback), this) == -1) {
            AtomLogError("Task Runner ALooper_addFd error");
        }
    }

    bool AndroidTaskRunner::onFileDescriptorEvent(int signalled_fd, int events) {
        if (!(events & (ALOOPER_EVENT_INPUT | ALOOPER_EVENT_ERROR |
                        ALOOPER_EVENT_HANGUP | ALOOPER_EVENT_INVALID))) {
            return true;
        }
        std::function<void()> task;
        {
            std::lock_guard<std::mutex> lock(m_lock);
            auto it = m_watch_tasks.find(signalled_fd);
            if (it == m_watch_tasks.end())
                return false;
            task = it->second;
        }
        errno = 0;
        runTask(task);
        return true;
    }


    void AndroidTaskRunner::runTask(const std::function<void()> &task) {
        task();
    }

    void AndroidTaskRunner::runImmediateTask() {
        uint64_t unused = 0;
        if (read(m_eventFD, &unused, sizeof(unused)) != sizeof(unused) && errno != EAGAIN) {
            AtomLogError("read error");
        }

        // TODO: Add a separate work queue in case in case locking overhead
        // becomes an issue.
        bool has_next;
        std::function<void()> immediate_task;
        {
            std::lock_guard<std::mutex> lock(m_lock);
            if (m_immediate_tasks.empty())
                return;
            immediate_task = std::move(m_immediate_tasks.front());
            m_immediate_tasks.pop_front();
            has_next = !m_immediate_tasks.empty();
        }
        // Do another pass through the event loop even if we have immediate tasks to
        // run for fairness.
        if (has_next)
            scheduleImmediateWakeUp();
        errno = 0;
        runTask(immediate_task);
    }

    void AndroidTaskRunner::runDelayedTask() {
        uint64_t unused = 0;
        if (read(m_timerFD, &unused, sizeof(unused)) != sizeof(unused) &&
            errno != EAGAIN) {
            AtomLogError("read error");
        }

        std::function<void()> delayed_task;
        ThreadTimerInterval next_wake_up = 0;
        {
            std::lock_guard<std::mutex> lock(m_lock);
            if (m_delayed_tasks.empty())
                return;
            auto it = m_delayed_tasks.begin();
            if (ThreadTimer::now() < it->first) {
                return;
            }
            delayed_task = std::move(it->second);
            m_delayed_tasks.erase(it);
            if (!m_delayed_tasks.empty())
                next_wake_up = m_delayed_tasks.begin()->first;
        }
        if (next_wake_up)
            scheduleDelayedWakeUp(next_wake_up);
        errno = 0;
        runTask(delayed_task);
    }

}