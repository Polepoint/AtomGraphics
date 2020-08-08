//
// Created by neo on 2018/8/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_FONT_H
#define ATOMGRAPHICS_FONT_H

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "base/ATString.h"
#include "base/TypeDefine.h"
#include "math/FloatPoint.h"
#include "FallbackListCompositeKey.h"
#include "FontDescription.h"
#include "FontFallbackList.h"
#include "FontStyle.h"
#include "FontMetrics.h"
#include "ShapeCache.h"
#include "platform/AtomPlatformConfig.h"


#if PLATFORM(IOS)

#import <CoreGraphics/CGContext.h>

#elif PLATFORM(ANDROID)

#include <skia/core/SkTypeface.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkCanvas.h>

#endif

namespace AtomGraphics {

class Font final {

public:

    Font();

    Font(const Font &other);

    Font(const FontDescription &fd);

    ~Font() = default;

    const FontDescription &fontDescription() const {
        return font_description_;
    }

    void setFontDescription(const FontDescription &font_description) {
        font_description_ = font_description;
    }

    const FontFallbackList &fallbackList() const;

    const std::string &cachedCSSConfig() const;

    void cacheCSSConfig(std::string &);

    float widthForString(const base::String &) const;

    const FontData *PrimaryFontData() const {
        return m_fallbackList.fontDataAt(font_description_, 0);
    }

    ShapeCache *getShapeCache(const FontDescription &fontDescription) const;

    FallbackListCompositeKey compositeKey(const FontDescription &font_description) const;

    inline bool operator==(const Font &other) const {
        return this->font_description_ == other.font_description_;
    };

    inline bool operator!=(const Font &other) const {
        return !(*this == other);
    }

    Font &operator=(const Font &other) {
        if (this == &other)
            return *this;

        bool dirty = *(this) != other;

        if (dirty) {
            reset();
        }
        this->m_fallbackList = other.m_fallbackList;
        this->font_description_ = other.font_description_;
        this->shape_cache_ = other.shape_cache_;

        return *this;
    }

#if PLATFORM(IOS)

    void drawBidiText(CGContextRef, const base::String &text, const FloatPoint &position) const;

#elif PLATFORM(ANDROID)

    void drawBidiText(SkCanvas *, const base::String &text, const FloatPoint &position, const SkPaint &) const;

#endif

private:
    FontFallbackList m_fallbackList;
    mutable FontMetrics m_fontMetrics;

    std::string m_cssConfig;

    FontDescription font_description_;

    mutable ShapeCache *shape_cache_{nullptr};

    float widthForCharacters(const UChar *characters, unsigned int length) const;

    void reset();

};

}

#endif //ATOMGRAPHICS_FONT_H
