//
// Created by neo on 2019-08-23.
//

#include "thread/GraphicsThread.h"
#include <vector>
#include "ScriptTimerController.h"

#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include "javascript/v8/V8JavaScriptInterface.h"
#include "javascript/v8/V8Platform.h"

#endif


namespace AtomGraphics {

ScriptTimerController::ScriptTimerController()
        : m_timer(GraphicsThread::GraphicsThreadTaskRunner(),
                  *this,
                  &ScriptTimerController::timerFired) {

}

static ScriptTimerController *s_instance;

ScriptTimerController *ScriptTimerController::Instance() {
    if (!s_instance) {
        s_instance = new ScriptTimerController;
    }

    return s_instance;
}

long ScriptTimerController::CreateTimerID(long millionSeconds, bool repeat, const std::function<void()> &function,
                                          void *userInfo) {
    auto scriptTimer = ScriptTimer::CreateTimer(millionSeconds, repeat, function, userInfo);
    ScriptTimerController *instance = Instance();
    long timerID = scriptTimer->m_timerID;

    instance->m_scriptTimers.emplace(scriptTimer->m_schedule_time, std::move(scriptTimer));
    if (instance->m_nextScheduleFiredTime != instance->m_scriptTimers.begin()->first) {
        instance->m_nextScheduleFiredTime = instance->m_scriptTimers.begin()->first;
        instance->m_timer.setNextFireTime(instance->m_nextScheduleFiredTime);
    }

    return timerID;
}

void ScriptTimerController::RemoveTimerID(long timerID) {
    ScriptTimerController *instance = Instance();
    for (auto &it : instance->m_scriptTimers) {
        if (it.second->m_timerID == timerID) {
            it.second->m_cancel = true;
            return;
        }
    }
}

void ScriptTimerController::timerFired() {
#if PLATFORM(ANDROID)
    using namespace JS;
    v8::Isolate *isolate = V8Platform::SharedPlatform()->getIsolate();
    v8::Context::Scope contextScope(
            V8JavaScriptInterface::SharedInterface()->context()->getGlobalContext().Get(isolate));
#endif
    while (!m_scriptTimers.empty() && m_scriptTimers.begin()->first < Clock::now()) {
        std::unique_ptr<ScriptTimer> scriptTimer = std::move(m_scriptTimers.begin()->second);
        m_scriptTimers.erase(m_scriptTimers.begin());
        if (scriptTimer->m_cancel) {
            continue;
        }
        scriptTimer->fired();
        if (scriptTimer->m_repeats) {
            m_scriptTimers.emplace(Clock::now() + scriptTimer->m_interval_time, std::move(scriptTimer));
        }
    }

    if (!m_scriptTimers.empty()) {
        m_nextScheduleFiredTime = m_scriptTimers.begin()->first;
        m_timer.setNextFireTime(m_nextScheduleFiredTime);
    }
}

}