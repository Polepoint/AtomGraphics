//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_DISPLAYREFRESHMONITOR_H
#define ATOMGRAPHICS_DISPLAYREFRESHMONITOR_H

#include <set>
#include "DisplayRefreshMonitorClient.h"

namespace AtomGraphics {

class DisplayRefreshMonitor {

public:
    
    void addClient(DisplayRefreshMonitorClient *);
    
    void removeClient(DisplayRefreshMonitorClient *);
    
    void didUpdateLayers();

private:
    std::set<DisplayRefreshMonitorClient *> m_clients;
    std::set<DisplayRefreshMonitorClient *> m_pendingInsertClients;
    std::set<DisplayRefreshMonitorClient *> m_pendingDeleteClients;
};
}

#endif //ATOMGRAPHICS_DISPLAYREFRESHMONITOR_H
