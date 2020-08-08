//
// Created by neo on 2018/4/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_TIMERBASE_H
#define ATOMGRAPHICS_TIMERBASE_H

#include "base/Clock.h"

namespace AtomGraphics {

class TimerBase {

public:
    
    virtual void start(TimeInterval nextFireInterval, TimeInterval repeatInterval) {
    
    };
    
    virtual void startRepeating(TimeInterval repeatInterval) {
    
    };
    
    
    virtual void startOneShot(TimeInterval delay) {
    };
    
    virtual void stop() {
    };
    
    virtual bool isActive() {
        return false;
    };
    
    virtual bool isValidate() {
        return false;
    };
    
    virtual void fired() {
    
    };
    
    virtual void setNextFireTime(TimeInterval newTime) {
    
    };

protected:
    TimeInterval m_nextFireTime{0};
    TimeInterval m_repeatInterval{0};
    bool m_validate{true};
};
}


#endif //ATOMGRAPHICS_TIMERBASE_H
