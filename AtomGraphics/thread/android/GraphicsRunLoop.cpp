//
// Created by neo on 2018/7/27.
//

#include <mutex>
#include <android/looper.h>
#include "thread/GraphicsRunLoop.h"

namespace AtomGraphics {

    static GraphicsRunLoop *sharedRunLoop;

    static ALooper *currentLooper = nullptr;

    GraphicsRunLoop *GraphicsRunLoop::SharedGraphicsLoop() {
        return sharedRunLoop;
    }

    void GraphicsRunLoop::InitSharedGraphicsLoop() {
        if (sharedRunLoop) {
            delete sharedRunLoop;
        }
        sharedRunLoop = new GraphicsRunLoop();
    }

    bool GraphicsRunLoop::isCurrentTimerRunLoop() {
        ALooper *current = ALooper_forThread();
        return current == currentLooper;
    }

    GraphicsRunLoop::GraphicsRunLoop() : m_looper(ALooper_prepare(0)) {
        ALooper_acquire(m_looper);
        currentLooper = m_looper;
    }

    GraphicsRunLoop::~GraphicsRunLoop() {
        if (m_looper) {
            ALooper_release(m_looper);
        }
    }

    void GraphicsRunLoop::run() {
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

    void GraphicsRunLoop::quit() {
        std::lock_guard<std::mutex> lock(m_lock);
        m_quit = true;
        ALooper_wake(m_looper);
    }

    ALooper *GraphicsRunLoop::getLooper() {
        return m_looper;
    }
}