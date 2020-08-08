//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#include <map>
#include "DisplayRefreshMonitor.h"
#import "GraphicsContentFlushController.h"

namespace AtomGraphics {

void DisplayRefreshMonitor::addClient(DisplayRefreshMonitorClient *client) {
    m_pendingInsertClients.insert(client);
}

void DisplayRefreshMonitor::removeClient(DisplayRefreshMonitorClient *client) {
    m_pendingDeleteClients.erase(client);
}

void DisplayRefreshMonitor::didUpdateLayers() {
    if (!m_pendingInsertClients.empty()) {
        for (DisplayRefreshMonitorClient *client: m_pendingInsertClients) {
            m_clients.insert(client);
        }
    }

    if (!m_pendingDeleteClients.empty()) {
        for (DisplayRefreshMonitorClient *client: m_pendingDeleteClients) {
            m_clients.erase(client);
        }
    }

    if (m_clients.empty()) {
        return;
    }

    for (DisplayRefreshMonitorClient *client: m_clients) {
        client->fireDisplayRefreshIfNeeded();
    }
}

}