//
// Created by neo on 2019-08-31.
//

#include "ContentViewProxy.h"
#import "PlatformTileLayer.h"


namespace AtomGraphics {

ContentViewProxy::ContentViewProxy() {
    PlatformLayer *platformLayer = new PlatformTileLayer();
    GraphicsLayer *graphicsLayer = new GraphicsLayer(std::unique_ptr<PlatformLayer>(platformLayer));
    _rootLayer.reset(graphicsLayer);
    _page.reset(new GraphicsPage(graphicsLayer));
}

void ContentViewProxy::destroy() {
    delete this;
}

}