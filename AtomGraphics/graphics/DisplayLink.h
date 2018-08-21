//
// Created by neo on 2018/7/9.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_VIEWCONTENTDISPLAYLINKHANDLER_H
#define ATOMGRAPHICS_VIEWCONTENTDISPLAYLINKHANDLER_H


#include "GraphicsContentFlushController.h"

namespace AtomGraphics {

    class GraphicsContentFlushController;

    class DisplayLink {

    public:

        DisplayLink(GraphicsContentFlushController *flushController)
                : m_flushController(flushController) {
        }

        virtual void schedule() {

        };

        virtual void pause() {

        };

        virtual void displayLinkFired() final;

    protected:
        GraphicsContentFlushController *m_flushController;
    };


}


#endif //ATOMGRAPHICS_VIEWCONTENTDISPLAYLINKHANDLER_H
