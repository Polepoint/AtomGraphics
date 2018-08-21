//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_GRAPHICSPAGEMANAGER_H
#define ATOMGRAPHICS_GRAPHICSPAGEMANAGER_H


#include "GraphicsPage.h"

namespace AtomGraphics {
    class GraphicsPageManager {

    public:
        static void registerPage(GraphicsPage *page);

        static void unregisterPage(GraphicsPage *page);

        static GraphicsPage *getPageByID(long pageID);

    };

}


#endif //ATOMGRAPHICS_GRAPHICSPAGEMANAGER_H
