//
// Created by neo on 2019/2/12.
//

#include "ContentViewUpdateDispatcher.h"
#include "graphics/TransactionBunch.h"

namespace AtomGraphics {


void ContentViewUpdateDispatcher::registerContentView(ContentView *contentView) {
    m_contentViews.emplace(contentView);
}

void ContentViewUpdateDispatcher::unregisterContentView(ContentView *contentView) {
    auto it = m_contentViews.find(contentView);
    if (it != m_contentViews.end()) {
        m_contentViews.erase(it);
    }
}

void ContentViewUpdateDispatcher::contentViewReadyToRender(ContentView *contentView) {
    std::lock_guard<std::mutex> lock(m_lock);
    long pageID = contentView->m_page->pageID();
    auto it = m_contentViews.find(contentView);
    if (it != m_contentViews.end()) {
        m_renderingContentViews.insert(std::make_pair(pageID, contentView));
    }
}

void ContentViewUpdateDispatcher::contentViewStopRendering(ContentView *contentView) {
    std::lock_guard<std::mutex> lock(m_lock);
    m_renderingContentViews.erase(contentView->pageID());
}

void ContentViewUpdateDispatcher::dispatchTransaction(scoped_refptr<TransactionBunch> bunch) {
    std::map<long, ContentView *> contentViews;
    {
        std::lock_guard<std::mutex> lock(m_lock);
        contentViews = m_renderingContentViews;
    }

    for (auto page : contentViews) {
        Transaction *transaction = bunch->getTransaction(page.first);
        if (transaction) {
            page.second->applyTransaction(*transaction);
        }
    }
}

}