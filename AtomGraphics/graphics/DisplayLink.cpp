//
// Created by neo on 2018/7/29.
//


#include "DisplayLink.h"
#include "GraphicsContentFlushController.h"

namespace AtomGraphics {

void DisplayLink::displayLinkFired() {
    m_flushController->didRefreshDisplay();
}

}