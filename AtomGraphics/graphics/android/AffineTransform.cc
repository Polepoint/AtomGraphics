//
// Created by neo on 2018/9/4.
//

#include <skia/core/SkMatrix.h>
#include "graphics/AffineTransform.h"

namespace AtomGraphics {

AffineTransform::AffineTransform(const SkMatrix &matrix)
        : AffineTransform(matrix.getScaleX(),
                          matrix.getSkewY(),
                          matrix.getSkewX(),
                          matrix.getScaleY(),
                          matrix.getTranslateX(),
                          matrix.getTranslateY()) {

}

AffineTransform::operator SkMatrix() const {
    SkMatrix result;

    result.setScaleX(SkDoubleToScalar(m_a));
    result.setSkewX(SkDoubleToScalar(m_c));
    result.setTranslateX(SkDoubleToScalar(m_tx));

    result.setScaleY(SkDoubleToScalar(m_d));
    result.setSkewY(SkDoubleToScalar(m_b));
    result.setTranslateY(SkDoubleToScalar(m_ty));

    result.setPerspX(0);
    result.setPerspY(0);
    result.set(SkMatrix::kMPersp2, SK_Scalar1);

    return result;
}

}