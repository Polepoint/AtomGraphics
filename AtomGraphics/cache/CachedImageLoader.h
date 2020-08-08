//
// Created by neo on 2020/4/23.
//

#ifndef ATOMGRAPHICS_CACHEDIMAGELOADER_H
#define ATOMGRAPHICS_CACHEDIMAGELOADER_H

#include "CachedImageHandle.h"
#include "CachedImageRequest.h"

namespace AtomGraphics {

class CachedImageLoader {

public:

    static CachedImageHandle requestImage(CachedImageRequest &request);

private:

    static CachedImageHandle loadResource(CachedImageRequest &request);
};

}


#endif //ATOMGRAPHICS_CACHEDIMAGELOADER_H
