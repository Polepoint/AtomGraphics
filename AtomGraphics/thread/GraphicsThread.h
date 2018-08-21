//
// Created by neo on 2018/7/7.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_ATOMGRAPHICSRUNLOOP_H
#define ATOMGRAPHICS_ATOMGRAPHICSRUNLOOP_H

#include <functional>
#include "platform/AtomPlatformConfig.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

#include <thread/android/AndroidTaskRunner.h>

#endif

namespace AtomGraphics {

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

    class AndroidTaskRunner;

#endif

    class GraphicsThread {
    public:

        static void InitGraphicsThread();

        static void DispatchOnGraphicsQueue(std::function<void()> block);

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

        static void InitMainThread();

        static AndroidTaskRunner* uiTaskRunner();

#endif

    private:
        static void GraphicsThreadEnable();

    };
}


#endif //ATOMGRAPHICS_ATOMGRAPHICSRUNLOOP_H
