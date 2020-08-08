//
// Created by neo on 2019-08-31.
//

#ifndef ATOMGRAPHICS_CONTENTVIEWPROXY_H
#define ATOMGRAPHICS_CONTENTVIEWPROXY_H


#import "GraphicsLayer.h"

namespace AtomGraphics {

class ContentViewProxy {

public:

    ContentViewProxy();

    GraphicsPage *getPage() const {
        return _page.get();
    }

    GraphicsLayer *getRootLayer() const {
        return _rootLayer.get();
    }

    void destroy();

private:

    std::unique_ptr<GraphicsLayer> _rootLayer;
    std::shared_ptr<GraphicsPage> _page;

};


}

#endif //ATOMGRAPHICS_CONTENTVIEWPROXY_H
