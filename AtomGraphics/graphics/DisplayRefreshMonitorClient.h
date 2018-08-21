//
// Created by neo on 2018/7/4.
// Copyright (c) 2018 neo. All rights reserved.
//
#ifndef ATOMGRAPHICS_DISPLAYREFRESHMONITORCLIENT_H
#define ATOMGRAPHICS_DISPLAYREFRESHMONITORCLIENT_H

namespace AtomGraphics {

    class DisplayRefreshMonitorClient {

    public:

        void fireDisplayRefreshIfNeeded();

        virtual void displayRefreshFired();

    protected:
        bool m_scheduled;
    };
}

#endif //ATOMGRAPHICS_DISPLAYREFRESHMONITORCLIENT_H