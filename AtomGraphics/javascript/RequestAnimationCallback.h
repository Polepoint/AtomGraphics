//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_REQEUESTANIMATIONCALLBACK_H
#define ATOMGRAPHICS_REQEUESTANIMATIONCALLBACK_H

#include <functional>
#include "memory/RefCounted.h"
#include "ScriptAnimationFrameController.h"

namespace AtomGraphics {

class RequestAnimationCallback : public RefCounted<RequestAnimationCallback> {

public:

    static scoped_refptr<RequestAnimationCallback> Create(std::function<void()> block) {
        return WrapRefCounted(new RequestAnimationCallback(std::move(block)));
    }

    void call();

private:

    RequestAnimationCallback(std::function<void()> block);

    long m_id;
    bool m_cancelled{false};
    std::function<void()> m_block;

    friend class ScriptAnimationFrameController;
};
}


#endif //ATOMGRAPHICS_REQEUESTANIMATIONCALLBACK_H
