//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "DisplayRefreshMonitorClient.h"

namespace AtomGraphics {

void DisplayRefreshMonitorClient::fireDisplayRefreshIfNeeded() {
    if (!m_scheduled) {
        return;
    }
    displayRefreshFired();
}

void DisplayRefreshMonitorClient::displayRefreshFired() {

}

}