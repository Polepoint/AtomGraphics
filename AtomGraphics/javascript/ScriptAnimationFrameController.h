//
// Created by neo on 2018/6/27.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_SCRIPTANIMATIONFRAMECONTROLLER_H
#define ATOMGRAPHICS_SCRIPTANIMATIONFRAMECONTROLLER_H


#include <vector>
#include "graphics/DisplayRefreshMonitor.h"
#include "graphics/GraphicsPageContext.h"
#include "RequestAnimationCallback.h"
#include "thread/Timer.h"

namespace AtomGraphics {

    class RequestAnimationCallback;

    class ScriptAnimationFrameController : public DisplayRefreshMonitorClient {

    public:

        static ScriptAnimationFrameController *ControllerForContext(GraphicsPageContext *pageContext);

        ScriptAnimationFrameController();

        long addCallback(RequestAnimationCallback *callback);

        void cancelCallback(long callbackBlockID);

        void displayRefreshFired() override;


    private:

        std::vector<RequestAnimationCallback *> m_callbacks;

        void asyncFireCallbacks();
    };
}

#endif  //ATOMGRAPHICS_SCRIPTANIMATIONFRAMECONTROLLER_H