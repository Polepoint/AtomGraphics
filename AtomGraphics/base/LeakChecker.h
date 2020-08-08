//
// Created by neo on 2020/7/14.
//

#ifndef ATGPROJECT_LEAKCHECKER_H
#define ATGPROJECT_LEAKCHECKER_H

#include <set>
#include <mutex>
#include "AtomLog.h"

namespace AtomGraphics {

class LeakChecker {

public:

    static LeakChecker *singleton() {
        static LeakChecker *debugger;

        static std::once_flag onceFlag;
        std::call_once(onceFlag, [] {
            debugger = new LeakChecker;
        });

        return debugger;
    }

    void recordAlloc(long location) {
        std::lock_guard<std::mutex> lock(m_lock);
        m_locations.emplace(location);
    }

    void recordRelease(long location) {
        std::lock_guard<std::mutex> lock(m_lock);
        auto it = m_locations.find(location);
        if (it == m_locations.end()) {
            AtomLogError("wild pointer was released: 0x%lx", location);
        }

        m_locations.erase(location);
    }


private:

    std::mutex m_lock;
    std::set<long> m_locations;
};

}

#endif //ATGPROJECT_LEAKCHECKER_H
