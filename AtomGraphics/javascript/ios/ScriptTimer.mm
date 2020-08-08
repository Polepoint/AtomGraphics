//
// Created by neo on 2018/11/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "ScriptTimer.h"

namespace AtomGraphics {

std::unique_ptr<ScriptTimer> ScriptTimer::CreateTimer(long millionSeconds, bool repeat, const std::function<void()> &function, void *userInfo) {
    return std::unique_ptr<ScriptTimer>(new ScriptTimer(millionSeconds, repeat, function));
}

}