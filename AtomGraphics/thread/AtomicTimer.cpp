//
// Created by Amazing on 2018-12-21.
//

#include "AtomicTimer.h"


namespace AtomGraphics {

bool AtomicTimer::isActive() {
    std::lock_guard<std::mutex> lock(m_lock);
    return Timer::isActive();
}

void AtomicTimer::fired() {
    std::lock_guard<std::mutex> lock(m_lock);
    Timer::fired();
}

}