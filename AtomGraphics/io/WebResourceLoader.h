//
// Created by neo on 2020/4/26.
//

#ifndef ATOMGRAPHICS_WEBRESOURCELOADER_H
#define ATOMGRAPHICS_WEBRESOURCELOADER_H


#include <functional>
#include "memory/ScopedRefPtr.h"
#include "io/SharedBuffer.h"
#include "node/canvas/ImageBitmap.h"

namespace AtomGraphics {

class CachedImage;

class ResourceRequest;

class WebResourceLoader {

public:

    static void loadResource(
            ResourceRequest &request,
            std::function<void(scoped_refptr<SharedBuffer>,
                               float, float,
                               ImageBitmapConfiguration)> completeCallback);

};

}


#endif //ATOMGRAPHICS_WEBRESOURCELOADER_H
