//
// Created by neo on 2018/11/2.
//

#ifndef ATOMGRAPHICS_V8SCRIPTTIMER_H
#define ATOMGRAPHICS_V8SCRIPTTIMER_H


#include <v8/v8.h>
#include "javascript/ScriptTimer.h"

namespace AtomGraphics {

class V8ScriptTimer : public ScriptTimer {

public:

    V8ScriptTimer(long millionSeconds, bool repeat, const std::function<void()> &function,
                  v8::Global<v8::Function> *functionPtr);

    virtual ~V8ScriptTimer();

private:
    v8::Global<v8::Function> *m_functionHolder;
};

}


#endif //ATOMGRAPHICS_V8SCRIPTTIMER_H
