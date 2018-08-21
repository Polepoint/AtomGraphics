//
// Created by neo on 2018/7/24.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSRUNLOOP_H
#define ATOMGRAPHICS_GRAPHICSRUNLOOP_H

#include "platform/AtomPlatformConfig.h"

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

#include <CoreFoundation/CoreFoundation.h>

#endif

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

#include <android/looper.h>
#include <mutex>

#endif

namespace AtomGraphics {

    class GraphicsRunLoop {

    public:

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_IOS

        static void setTimerRunLoop(CFRunLoopRef globalTimerRunLoop);

        static CFRunLoopRef getTimerRunLoop();

#endif

#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID

        virtual ~GraphicsRunLoop();

        static void InitSharedGraphicsLoop();

        static GraphicsRunLoop *SharedGraphicsLoop();

        GraphicsRunLoop();

        ALooper *getLooper();

        void run();

        void quit();

#endif

        static bool isCurrentTimerRunLoop();


    private:
#if ATOM_TARGET_PLATFORM == ATOM_PLATFORM_ANDROID
        ALooper *m_looper;
        std::mutex m_lock;
        bool m_quit;
#endif

    };

}


#endif //ATOMGRAPHICS_GRAPHICSRUNLOOP_H
