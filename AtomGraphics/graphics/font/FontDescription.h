//
// Created by neo on 2019/4/2.
//

#ifndef ATOMGRAPHICS_FONTDESCRIPTION_H
#define ATOMGRAPHICS_FONTDESCRIPTION_H

#include <string>
#include "graphics/font/FontStyle.h"
#include "platform/AtomPlatformConfig.h"
#include "FontCacheKey.h"
#include "FontFamily.h"
#include "FontStyle.h"

#if PLATFORM(ANDROID)

#include <skia/core/SkFontStyle.h>

#endif

#if PLATFORM(IOS)

#include "memory/ios/RetainPtr.h"
#include <CoreText/CoreText.h>

#endif

namespace AtomGraphics {

class FontDescription {

public:

    FontDescription();

    FontDescription(const FontDescription &) = default;

    bool operator==(const FontDescription &) const;

    void SetFamily(const FontFamily &family) { family_list_ = family; }

    const FontFamily &Family() const {
        return family_list_;
    }

    const FontFamily *FamilyAt(int index) const;

    float getFontSize() const {
        return m_fontSize;
    }

    float ComputedSize() const { return m_fontSize; }

    void setFontSize(float fontSize) {
        m_fontSize = fontSize;
    }

    bool isSmallCaps() const {
        return m_smallCaps;
    }

    void setSmallCaps(bool smallCaps) {
        m_smallCaps = smallCaps;
    }

    void setWeight(size_t weight) {
        weight_ = weight;
    }

    size_t Weight() const {
        return weight_;
    }

    void setSlant(Slant slant) { slant_ = slant; };

    Slant slant() const { return slant_; }

    Slant getSlant() const {
        return slant_;
    }

    FontCacheKey CacheKey(const std::string &family) const {
        unsigned options =
                static_cast<unsigned>(weight_) << 3 |
                        static_cast<unsigned>(m_smallCaps) << 2 |
                        static_cast<unsigned>(slant_);

        return FontCacheKey(m_fontSize, options, family);
    }

#if PLATFORM(IOS)

    RetainPtr<CTFontDescriptorRef> ctFontDescriptor() const;

#endif

#if PLATFORM(ANDROID)

    SkFontStyle skiaFontStyle() const;

#endif


private:

    FontFamily family_list_;

    float m_fontSize;
    bool m_smallCaps;
    size_t weight_;
    Slant slant_;

    friend class FallbackListCompositeKey;
};

}


#endif //ATOMGRAPHICS_FONTDESCRIPTION_H
