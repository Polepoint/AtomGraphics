//
// Created by neo on 2019/2/12.
//

#ifndef ATOMGRAPHICS_CONTENTVIEWUPDATEDISPATCHER_H
#define ATOMGRAPHICS_CONTENTVIEWUPDATEDISPATCHER_H

#include "ContentView.h"
#include <map>
#include <set>

namespace AtomGraphics {

class ContentViewUpdateDispatcher {

public:

    static ContentViewUpdateDispatcher *SharedInstance() {
        static ContentViewUpdateDispatcher *sharedInstance;
        if (!sharedInstance) {
            sharedInstance = new ContentViewUpdateDispatcher();
        }

        return sharedInstance;
    }

    void registerContentView(ContentView *contentView);

    void unregisterContentView(ContentView *contentView);

    void contentViewReadyToRender(ContentView *contentView);

    void contentViewStopRendering(ContentView *contentView);

    void dispatchTransaction(scoped_refptr<TransactionBunch> bunch);

private:
    std::mutex m_lock;
    std::set<ContentView *> m_contentViews;
    std::map<long, ContentView *> m_renderingContentViews;

};

}


#endif //ATOMGRAPHICS_CONTENTVIEWUPDATEDISPATCHER_H
