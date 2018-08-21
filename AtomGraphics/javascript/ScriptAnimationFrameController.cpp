//
// Created by neo on 2018/6/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <map>
#include "graphics/GraphicsContentFlushController.h"
#include "ScriptAnimationFrameController.h"

namespace AtomGraphics {

    static long timerCount = 0;

    static std::map<GraphicsPageContext *, ScriptAnimationFrameController *> controllerMap;

    ScriptAnimationFrameController *
    ScriptAnimationFrameController::ControllerForContext(GraphicsPageContext *pageContext) {
        ScriptAnimationFrameController *controller = controllerMap[pageContext];
        if (!controller) {
            GraphicsContentFlushController *flushController = pageContext->flushController();
            DisplayRefreshMonitor *refreshMonitor = flushController->refreshMonitor();
            if (!refreshMonitor) {
                refreshMonitor = new DisplayRefreshMonitor();
                flushController->registerDisplayRefreshMonitor(refreshMonitor);
            }
            controller = new ScriptAnimationFrameController();
            refreshMonitor->addClient(controller);
            controllerMap[pageContext] = controller;
        }

        return controller;
    }

    ScriptAnimationFrameController::ScriptAnimationFrameController() {
        m_scheduled = true;
    }

    long ScriptAnimationFrameController::addCallback(RequestAnimationCallback *callback) {
        m_callbacks.push_back(callback);
        callback->m_id = ++timerCount;
        callback->m_firedOrCancelled = false;
        return timerCount;
    }

    void ScriptAnimationFrameController::cancelCallback(long callbackID) {
        for (auto it = m_callbacks.begin(); it != m_callbacks.end(); it++) {
            RequestAnimationCallback *callback = *(it.base());
            if (callback->m_id == callbackID) {
                callback->m_firedOrCancelled = true;
                break;
            }
        }
    }

    void ScriptAnimationFrameController::displayRefreshFired() {
        ScriptAnimationFrameController *thisRef = this;
        GraphicsThread::DispatchOnGraphicsQueue([thisRef]() {
            thisRef->asyncFireCallbacks();
        });
    }

    void ScriptAnimationFrameController::asyncFireCallbacks() {
        auto callbacks = m_callbacks;
        m_callbacks.clear();
        for (auto callback :callbacks) {
            callback->call();
            delete callback;
        }
    }
}