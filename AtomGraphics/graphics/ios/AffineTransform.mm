//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "AffineTransform.h"

namespace AtomGraphics {

AffineTransform::operator CGAffineTransform() const {
    return CGAffineTransformMake(m_a, m_b, m_c, m_d, m_tx, m_ty);
}

AffineTransform::AffineTransform(const CGAffineTransform &at)
        : m_a(at.a), m_b(at.b), m_c(at.c), m_d(at.d), m_tx(at.tx), m_ty(at.ty) {
    
}

}
