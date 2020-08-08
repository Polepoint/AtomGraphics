//
// Created by neo on 2020/4/23.
//

#ifndef ATOMGRAPHICS_CACHEDIMAGECLIENT_H
#define ATOMGRAPHICS_CACHEDIMAGECLIENT_H

namespace AtomGraphics {

class CachedImage;

class CachedImageClient {

public:

    virtual void notifyFinished(CachedImage &) = 0;

};

}


#endif //ATOMGRAPHICS_CACHEDIMAGECLIENT_H
