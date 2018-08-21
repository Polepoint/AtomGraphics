//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_REQEUESTANIMATIONCALLBACK_H
#define ATOMGRAPHICS_REQEUESTANIMATIONCALLBACK_H

#include <functional>
#include "ScriptAnimationFrameController.h"

namespace AtomGraphics {

    class RequestAnimationCallback {

    public:
        RequestAnimationCallback(std::function<void()> block);

    public:
        void call();

    private:
        long m_id;
        bool m_firedOrCancelled;
        std::function<void()> m_block;

        friend class ScriptAnimationFrameController;
    };
}


#endif //ATOMGRAPHICS_REQEUESTANIMATIONCALLBACK_H
