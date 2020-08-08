//
// Created by neo on 2018/6/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <map>
#include "graphics/GraphicsPageManager.h"
#include "graphics/GraphicsContentFlushController.h"
#include "ScriptAnimationFrameController.h"
#include "thread/GraphicsThread.h"
#include "platform/AtomPlatformConfig.h"

namespace AtomGraphics {

static long s_callbackIndex = 0;

ScriptAnimationFrameController *ScriptAnimationFrameController::SharedInstance() {
    static ScriptAnimationFrameController *controller;
    if (!controller) {
        GraphicsContentFlushController *flushController = GraphicsContentFlushController::SharedInstance();
        controller = new ScriptAnimationFrameController();
        flushController->refreshMonitor()->addClient(controller);
    }

    return controller;
}

ScriptAnimationFrameController::ScriptAnimationFrameController() {
    m_scheduled = true;
}

long ScriptAnimationFrameController::addCallback(scoped_refptr<RequestAnimationCallback> callback) {
    callback->m_id = ++s_callbackIndex;
    m_callbackLists.emplace(s_callbackIndex, callback);
    return s_callbackIndex;
}

void ScriptAnimationFrameController::cancelCallback(long callbackID) {
    auto it = m_callbackLists.find(callbackID);
    if (it != m_callbackLists.end()) {
        it->second->m_cancelled = true;
    }
}

void ScriptAnimationFrameController::displayRefreshFired() {

#if PLATFORM(ANDROID)

    GraphicsThread::DispatchOnGraphicsQueueWithJSContextScope([this]() {
        this->asyncFireCallbacks();
    });

#elif PLATFORM(IOS)

    GraphicsThread::DispatchOnGraphicsQueue([this]() {
        this->asyncFireCallbacks();
    });

#endif

}

void ScriptAnimationFrameController::asyncFireCallbacks() {
    std::map<long, scoped_refptr<RequestAnimationCallback>> callbackListsPendingToExecute;
    callbackListsPendingToExecute.swap(m_callbackLists);
    m_callbackLists.clear();
    for (auto &it : callbackListsPendingToExecute) {
        if (!it.second->m_cancelled) {
            it.second->call();
        }
    }
}

}