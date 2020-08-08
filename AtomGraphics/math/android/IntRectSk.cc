//
// Created by neo on 2019/1/6.
//

#include "math/IntRect.h"

namespace AtomGraphics {

IntRect::IntRect(const SkIRect &rect)
        : IntRect(rect.x(), rect.y(), rect.width(), rect.height()) {

}

IntRect::operator SkIRect() const {
    return SkIRect::MakeXYWH(origin.x, origin.y, size.width, size.height);
}

IntRect::operator SkRect() const {
    return SkRect::MakeXYWH(
            SkIntToScalar(origin.x),
            SkIntToScalar(origin.y),
            SkIntToScalar(size.width),
            SkIntToScalar(size.height)
    );
}

}