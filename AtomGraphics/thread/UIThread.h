//
// Created by neo on 2019/3/17.
//

#ifndef ATOMGRAPHICS_UITHREAD_H
#define ATOMGRAPHICS_UITHREAD_H

#include "TaskRunner.h"

namespace AtomGraphics {

class UIThread {

public:

    static void InitUIThread();

    static TaskRunner *UIThreadTaskRunner();

    static bool IsMainThread();

};

}


#endif //ATOMGRAPHICS_UITHREAD_H
