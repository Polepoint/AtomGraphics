//
// Created by neo on 2020/4/25.
//

#ifndef ATOMGRAPHICS_CACHEDIMAGEREQUEST_H
#define ATOMGRAPHICS_CACHEDIMAGEREQUEST_H

#include "io/ResourceRequest.h"

namespace AtomGraphics {

class CachedImageRequest : public ResourceRequest {

public:

    CachedImageRequest(const std::string &url);
};

}


#endif //ATOMGRAPHICS_CACHEDIMAGEREQUEST_H
