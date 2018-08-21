//
// Created by neo on 2018/7/18.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "GraphicsPageManager.h"

namespace AtomGraphics {

    std::set<GraphicsPage *> pages;

    void GraphicsPageManager::registerPage(GraphicsPage *page) {
        pages.insert(page);
    }

    void GraphicsPageManager::unregisterPage(GraphicsPage *page) {
        pages.erase(page);
    }

    GraphicsPage *GraphicsPageManager::getPageByID(long pageID) {
        for (auto page : pages) {
            if (page->pageID() == pageID)
                return page;
        }

        return nullptr;
    }
}