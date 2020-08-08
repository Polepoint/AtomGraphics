//
// Created by neo on 2018/11/14.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FLOATSIZE_H
#define ATOMGRAPHICS_FLOATSIZE_H

#include "platform/AtomPlatformConfig.h"
#include "Vec2.h"

#if PLATFORM(IOS)

#include <CoreGraphics/CoreGraphics.h>

#endif

namespace AtomGraphics {

class IntSize;

class FloatSize {

public:
    float width;
    float height;
public:
    
    operator Vec2() const {
        return Vec2(width, height);
    }

public:
    
    FloatSize();
    
    FloatSize(float width, float height);
    
    FloatSize(const FloatSize &other);
    
    FloatSize(const IntSize &intSize);
    
    explicit FloatSize(const Vec2 &point);
    
    FloatSize &operator=(const FloatSize &other);
    
    FloatSize &operator=(const Vec2 &point);
    
    FloatSize operator+(const FloatSize &right) const;
    
    void operator+=(const FloatSize& v);
    
    FloatSize operator-(const FloatSize &right) const;
    
    FloatSize operator*(float a) const;
    
    FloatSize operator/(float a) const;
    
    inline bool operator==(const FloatSize &point) const {
        return equals(point);
    }
    
    inline bool operator!=(const FloatSize &point) const {
        return !equals(point);
    }

#if PLATFORM(IOS)
    
    operator CGSize() const;

#endif
    
    void setSize(float width, float height);
    
    bool equals(const FloatSize &target) const;
    
    void scale(float scale);
    
    void scale(float scaleX, float scaleY);
    
    bool isEmpty() const {
        return width <= 0 || height <= 0;
    }
    
    static const FloatSize ZERO;
};

}

#endif //ATOMGRAPHICS_FLOATSIZE_H
