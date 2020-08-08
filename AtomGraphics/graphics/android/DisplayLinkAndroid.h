//
// Created by neo on 2018/7/26.
//

#ifndef ATOMGRAPHICS_DISPLAYLINKANDROID_H
#define ATOMGRAPHICS_DISPLAYLINKANDROID_H

#include <jni.h>
#include "graphics/DisplayLink.h"

namespace AtomGraphics {

class DisplayLinkAndroid final : public DisplayLink {

public:
    DisplayLinkAndroid(GraphicsContentFlushController *flushController);

    ~DisplayLinkAndroid();

    void schedule() override;

    void pause() override;

    bool isPaused() {
        return m_pause;
    }

private:
    bool m_pause{true};
    jobject m_displayLinkObj;
};
}


#endif //ATOMGRAPHICS_DISPLAYLINKANDROID_H
