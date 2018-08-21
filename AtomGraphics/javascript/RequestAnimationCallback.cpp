//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "RequestAnimationCallback.h"

namespace AtomGraphics {

    RequestAnimationCallback::RequestAnimationCallback(std::function<void()> block) : m_block(block) {

    }

    void RequestAnimationCallback::call() {
        if (m_firedOrCancelled) {
            return;
        }

        if (m_block) {
            m_block();
        }

        m_firedOrCancelled = true;
    }
}