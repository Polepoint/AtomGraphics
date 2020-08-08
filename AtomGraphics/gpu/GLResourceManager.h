//
// Created by neo on 2019/3/30.
//

#ifndef ATOMGRAPHICS_GPURESOURCEMANAGER_H
#define ATOMGRAPHICS_GPURESOURCEMANAGER_H

#include "GLResource.h"
#include "memory/ScopedRefPtr.h"

namespace AtomGraphics {

class GPUResourceManager {

public:

    static GPUResourceManager *sharedManger();

    scoped_refptr<GLResource> resourceForPage(long pageID);

    void initializePageResource(long pageID, ANativeWindow *window);

    void removePageResource(long pageID);

private:

    GPUResourceManager() {}

    std::map<long, scoped_refptr<GLResource>> m_resources;
};

}


#endif //ATOMGRAPHICS_GPURESOURCEMANAGER_H
