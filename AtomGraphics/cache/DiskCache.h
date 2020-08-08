//
// Created by neo on 2020/4/26.
//

#ifndef ATOMGRAPHICS_DISKCACHE_H
#define ATOMGRAPHICS_DISKCACHE_H

#include "memory/ScopedRefPtr.h"
#include "io/SharedBuffer.h"
#include "CachedImageRequest.h"
#include "CachedImageHandle.h"

namespace AtomGraphics {

class DiskCache {

public:

    CachedImageHandle resourceForRequest(const CachedImageRequest &request);

    void add(const CachedImageHandle &);

private:

    static const char *kCacheFolderName;

    bool m_cachePathInitialized{false};

    void checkCachePath();

    scoped_refptr<SharedBuffer> dataBufferFromFile(const char *fileName);

    void saveBufferDataToFile(const char *file, scoped_refptr<SharedBuffer> bufferData);

};

}


#endif //ATOMGRAPHICS_DISKCACHE_H
