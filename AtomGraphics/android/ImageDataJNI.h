//
// Created by neo on 2019-09-28.
//

#ifndef ATOMGRAPHICS_IMAGEDATAJNI_H
#define ATOMGRAPHICS_IMAGEDATAJNI_H

#include "graphics/ImageData.h"

namespace AtomGraphics {

class ImageDataJNI {

public:

    ImageDataJNI(scoped_refptr<ImageData> imageData);

    ImageData *getImageData() const {
        return m_imageData.get();
    }


private:
    scoped_refptr<AtomGraphics::ImageData> m_imageData;

};

}

#endif //ATOMGRAPHICS_IMAGEDATAJNI_H
