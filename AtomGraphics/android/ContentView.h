//
// Created by neo on 2019/1/8.
//

#ifndef ATOMGRAPHICS_CONTENTVIEW_H
#define ATOMGRAPHICS_CONTENTVIEW_H

#include <jni.h>
#include <android/native_window.h>
#include "graphics/GraphicsPage.h"
#include "graphics/GraphicsLayer.h"
#include "graphics/android/ResourceProvider.h"
#include "math/IntSize.h"
#include "CompositingManager.h"

namespace AtomGraphics {

class ContentView {

public:

    ContentView(jobject jObj);

    ~ContentView();

    void destroy();

    void surfaceCreated(jobject surface, int width, int height);

    void surfaceChanged(jobject surface, int width, int height);

    void surfaceDestroyed();

    void updateVisibleBounds(int x, int y, int width, int height);

    void updateVisibility(bool visible);

    long pageID() const {
        if (!m_page) {
            return -1;
        }
        return m_page->pageID();
    }

private:

    std::atomic_long m_refCount{1};

    jobject m_jRef;
    IntSize m_contentSize{IntSize::ZERO};
    IntRect m_visibleRect{IntRect::ZERO};
    bool m_visible{false};
    ANativeWindow *m_window{nullptr};
    bool m_windowChanged{false};
    bool m_glSurfaceAvailable{false};
    std::unique_ptr<ResourceProvider> m_resourceProvider;

    std::unique_ptr<GraphicsLayer> m_layer;
    std::unique_ptr<GraphicsPage> m_page;

    std::unique_ptr<CompositingManager> m_compositingManager;
    std::shared_ptr<CompositingView> m_rootCompositingView;

    bool m_didFinishFirstTransaction{false};

    void ref();

    void deref();

    void ensureGPUResource();

    void updateVisibleContentRects(FloatRect visibleRect);

    void applyTransaction(const Transaction &transaction);

    void finishApplyTransaction();

    GraphicsPage *page();

    GraphicsLayer *layer();

    CompositingView *rootCompositingView();

    ResourceProvider *resourceProvider();

    friend class ContentViewUpdateDispatcher;
};

}


#endif //ATOMGRAPHICS_CONTENTVIEW_H
