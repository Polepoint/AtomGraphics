//
// Created by neo on 2018/7/26.
//

#ifndef ATOMGRAPHICS_PLATFORMLAYERTEXTUREVIEW_H
#define ATOMGRAPHICS_PLATFORMLAYERTEXTUREVIEW_H

#include <jni.h>
#include "graphics/PlatformLayer.h"

namespace AtomGraphics {

    class PlatformLayerTextureView : public PlatformLayer {

    public:
        PlatformLayerTextureView(jobject textureViewObj);

        void renderCommand(const char *);

    private:
        jobject m_textureViewObj;
    };

}


#endif //ATOMGRAPHICS_PLATFORMLAYERTEXTUREVIEW_H
