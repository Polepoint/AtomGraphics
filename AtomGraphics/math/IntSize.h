//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_INTSIZE_H
#define ATOMGRAPHICS_INTSIZE_H

#include <math.h>
#include "platform/AtomPlatformConfig.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkSize.h>

#endif

#include "FloatSize.h"

namespace AtomGraphics {

class IntSize {
public:
    int width;
    int height;

public:

    IntSize();

    IntSize(int width, int height);

    IntSize(size_t width, size_t height);

    IntSize(float width, float height);

    IntSize(const IntSize &other);

    explicit IntSize(const FloatSize &floatSize);

    IntSize &operator=(const IntSize &other);

    IntSize &operator=(const FloatSize &point);

    IntSize operator+(const IntSize &right) const;

    IntSize operator-(const IntSize &right) const;

    IntSize operator*(int a) const;

    IntSize operator/(int a) const;

    inline bool operator==(const IntSize &point) const {
        return equals(point);
    }

    inline bool operator!=(const IntSize &point) const {
        return !equals(point);
    }

    inline bool operator<(const IntSize &point) const {
        if (height != point.height) {
            return height < point.height;
        }

        if (width != point.width) {
            return width < point.width;
        }

        return false;
    }

    void setSize(int width, int height);

    bool equals(const IntSize &target) const;

    void scale(float scale);

    bool isEmpty() const {
        return width <= 0 || height <= 0;
    }

#if PLATFORM(ANDROID)

    operator SkISize() const;

#endif

    static const IntSize ZERO;
};

inline IntSize roundedIntSize(const FloatSize &p) {
    return IntSize(roundf(p.width), roundf(p.height));
}

inline IntSize expandedIntSize(const FloatSize &p) {
    return IntSize(ceilf(p.width), ceilf(p.height));
}

inline IntSize operator-(const IntSize &size) {
    return IntSize(-size.width, -size.height);
}

}


#endif //ATOMGRAPHICS_INTSIZE_H
