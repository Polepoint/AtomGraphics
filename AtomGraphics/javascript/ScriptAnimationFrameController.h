//
// Created by neo on 2018/6/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_SCRIPTANIMATIONFRAMECONTROLLER_H
#define ATOMGRAPHICS_SCRIPTANIMATIONFRAMECONTROLLER_H


#include <map>
#include <vector>
#include "graphics/DisplayRefreshMonitor.h"
#include "RequestAnimationCallback.h"
#include "thread/Timer.h"
#include "memory/ScopedRefPtr.h"

namespace AtomGraphics {

class RequestAnimationCallback;

class ScriptAnimationFrameController : public DisplayRefreshMonitorClient {

public:

    static ScriptAnimationFrameController *SharedInstance();

    ScriptAnimationFrameController();

    long addCallback(scoped_refptr <RequestAnimationCallback> callback);

    void cancelCallback(long callbackID);

    void displayRefreshFired() override;


private:

    std::map<long, scoped_refptr<RequestAnimationCallback>> m_callbackLists;

    void asyncFireCallbacks();
};
}

#endif  //ATOMGRAPHICS_SCRIPTANIMATIONFRAMECONTROLLER_H