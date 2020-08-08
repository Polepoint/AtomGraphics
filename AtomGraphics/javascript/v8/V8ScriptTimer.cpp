//
// Created by neo on 2018/11/2.
//

#include "V8ScriptTimer.h"

namespace AtomGraphics {

std::unique_ptr<ScriptTimer> ScriptTimer::CreateTimer(long millionSeconds, bool repeat,
                                                      const std::function<void()> &function, void *userInfo) {

    ScriptTimer *timer = new V8ScriptTimer(millionSeconds, repeat, function,
                                           static_cast<v8::Global<v8::Function> *>(userInfo));
    return std::unique_ptr<ScriptTimer>(timer);
}


V8ScriptTimer::V8ScriptTimer(long millionSeconds, bool repeat, const std::function<void()> &function,
                             v8::Global<v8::Function> *functionPtr)
        : ScriptTimer(millionSeconds, repeat, function), m_functionHolder(functionPtr) {

}

V8ScriptTimer::~V8ScriptTimer() {
    if (m_functionHolder) {
        delete m_functionHolder;
        m_functionHolder = nullptr;
    }
}

}
